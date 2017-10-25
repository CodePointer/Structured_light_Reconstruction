% Load parameters
clear;
load EpipolarPara.mat
load GeneralPara.mat
load ColorPara.mat

% Parameters for storage
fprintf('Initial...');
depth_vecs = cell(total_frame_num, 1);
cam_est_vecs = cell(total_frame_num, 1);
cam_obs_vecs = fun_ReadCamVecsFromFile(FilePath, ...
    CamInfo, ...
    total_frame_num);
[depth_vecs{1,1},corres_mat] = fun_InitDepthVec(FilePath, ...
    CamInfo, ...
    ProInfo, ...
    ParaSet);
flow_size = 3;
for flw_idx = 1:flow_size
    depth_vecs{flw_idx+1,1} = depth_vecs{1,1};
end
fprintf('finished.\n');

% Iteration Part
error_value = zeros(total_frame_num, max_iter_num);
alpha = 4;
theta = [1e4; 1e2];
envir_light = ParaTable.color(end);
for frm_idx = 1+flow_size:total_frame_num
    flw_sta = frm_idx - flow_size + 1;
    flw_end = frm_idx;

    % Set initial frm_idx depth
    depth_vecs{frm_idx,1} = depth_vecs{frm_idx-1,1} ...
        + (depth_vecs{frm_idx-1}-depth_vecs{frm_idx-2,1});

    % 1st Iteration
    fprintf('%d-%d:.', frm_idx, 1);
    [projected_vecmats, valid_indexs] ...
        = fun_ProjectedImage(depth_vecs(flw_sta:flw_end,1), ...
        ParaTable, ...
        CamInfo, ...
        ProInfo, ...
        EpiLine, ...
        ParaSet);
    cam_est_vecs(flw_sta:flw_end,1) = fun_CalculateEstVecs(projected_vecmats, ...
        envir_light);
    error_value(frm_idx, 1) = fun_ErrorFunction(cam_est_vecs(flw_sta:flw_end,1), ...
        cam_obs_vecs(flw_sta:flw_end,1), ...
        depth_vecs(flw_sta:flw_end,1), ...
        theta, ...
        ParaSet);
    fprintf('.');
    fprintf('error=%.2f\n', error_value(frm_idx,1));

    % Iteration Part
    for itr_idx = 2:max_iter_num
        fprintf('%d-%d', frm_idx, itr_idx);
        % Calculate Derv value
        projected_derv_vecmats = fun_ProjectedImageDerv(projected_vecmats, ...
            valid_indexs, ...
            depth_vecs(flw_sta:flw_end,1), ...
            ParaTable, ...
            CamInfo, ...
            ProInfo, ...
            EpiLine, ...
            ParaSet);
        norm_derv_vecs = fun_ErrorFunctionDerv(cam_est_vecs(flw_sta:flw_end,1), ...
            cam_obs_vecs(flw_sta:flw_end,1), ...
            projected_derv_vecmats, ...
            depth_vecs(flw_sta:flw_end,1), ...
            theta, ...
            ParaSet);
        test_depth_vecs ...
            = fun_SetNewDepthFromStep(depth_vecs(flw_sta:flw_end), ...
            alpha, ...
            norm_derv_vecs);
        fprintf('.');
        % Calculate new value
        [test_projected_vecmats, test_valid_indexs] ...
            = fun_ProjectedImage(depth_vecs(flw_sta:flw_end,1), ...
            ParaTable, ...
            CamInfo, ...
            ProInfo, ...
            EpiLine, ...
            ParaSet);
        test_cam_est_vecs = fun_CalculateEstVecs(test_projected_vecmats, ...
            envir_light);
        error_value(frm_idx,itr_idx) = fun_ErrorFunction(test_cam_est_vecs, ...
            cam_obs_vecs(flw_sta:flw_end,1), ...
            test_depth_vecs, ...
            theta, ...
            ParaSet);
        % Accept or Reject
        if error_value(frm_idx,itr_idx) > error_value(frm_idx,itr_idx-1)
            alpha = alpha * 0.5;
            error_value(frm_idx,itr_idx) = error_value(frm_idx,itr_idx-1);
            fprintf('.reject. alpha->%.4f\n', alpha);
        else
            projected_vecmats = test_projected_vecmats;
            valid_indexs = test_valid_indexs;
            cam_est_vecs(flw_sta:flw_end,1) = test_cam_est_vecs;
            depth_vecs(flw_sta:flw_end,1) = test_depth_vecs;
            fprintf('.error=%.2f\t%.2f\n', error_value(frm_idx,itr_idx), ...
                error_value(frm_idx,itr_idx)-error_value(frm_idx,itr_idx-1));
            if error_value(frm_idx,itr_idx-1) - error_value(frm_idx,itr_idx) < 1e4
                break;
            end
        end
    end

    % Output finished point cloud
    fid_res = fopen([FilePath.output_file_name, num2str(flw_sta-1), '.txt'], 'w+');
    for cvec_idx = 1:CamInfo.RANGE_HEIGHT*CamInfo.RANGE_WIDTH
        h_cam = ParaSet.coord_cam(cvec_idx,1);
        w_cam = ParaSet.coord_cam(cvec_idx,2);
        x_cam = w_cam + CamInfo.range_mat(1,1) - 1;
        y_cam = h_cam + CamInfo.range_mat(2,1) - 1;

        z_wrd = depth_vecs{frm_idx,1}(cvec_idx);
        x_wrd = (x_cam - CalibMat.cam(1,3)) / CalibMat.cam(1,1) * z_wrd;
        y_wrd = (y_cam - CalibMat.cam(2,3)) / CalibMat.cam(2,2) * z_wrd;
        fprintf(fid_res, '%.2f %.2f %.2f \n', x_wrd, y_wrd, z_wrd);
    end
    fclose(fid_res);
end

% Output other frms
% blabla

save status.mat
