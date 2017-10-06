% Load parameters
clear;
load EpipolarPara.mat
load GeneralPara.mat

% Read information of 1st frame
cam_mats = cell(total_frame_num, 1);
depth_mats = cell(total_frame_num, 1);
valid_mats = cell(total_frame_num, 1);
corres_mats = cell(total_frame_num, 1);
[depth_mats{1,1}, corres_mats{1,1}, optical_mat] = fun_InitDepthMat(FilePath, ...
    CamInfo, ...
    ProInfo, ...
    ParaSet);
tmp_image = double(imread([FilePath.main_file_path, ...
    FilePath.img_file_path, ...
    FilePath.img_file_name, ...
    num2str(1), ...
    FilePath.img_file_suffix]));
cam_mats{1, 1} = tmp_image(CamInfo.cam_range(2,1):CamInfo.cam_range(2,2), ...
    CamInfo.cam_range(1,1):CamInfo.cam_range(1,2));
tmp_image = double(imread([FilePath.main_file_path, ...
    FilePath.img_file_path, ...
    FilePath.img_file_name, ...
    num2str(12), ...
    FilePath.img_file_suffix]));
cam_mats{2, 1} = tmp_image(CamInfo.cam_range(2,1):CamInfo.cam_range(2,2), ...
    CamInfo.cam_range(1,1):CamInfo.cam_range(1,2));
opt_mat = optical_mat(CamInfo.cam_range(2,1):CamInfo.cam_range(2,2), ...
    CamInfo.cam_range(1,1):CamInfo.cam_range(1,2));

show_mat = zeros(CamInfo.RANGE_HEIGHT, CamInfo.RANGE_WIDTH, 3);
show_mat(:, :, 1) = double(opt_mat) / 255;
show_mat(:, :, 2) = show_mat(:, :, 1);
show_mat(:, :, 3) = show_mat(:, :, 1);
for h = 1:51
    for w = 1:51
        h_c = corres_mats{1,1}{h,w}(1,1) - CamInfo.cam_range(2,1) + 1;
        w_c = corres_mats{1,1}{h,w}(1,2) - CamInfo.cam_range(1,1) + 1;
        show_mat(h_c, w_c, :) = 0.0;
        show_mat(h_c, w_c, 1) = 1.0;
    end
end

epi_corres_mat = cell(51,51);
for h_pro = 1:ProInfo.RANGE_HEIGHT
    for w_pro = 1:ProInfo.RANGE_WIDTH
        pvec_idx = (h_pro-1)*ProInfo.RANGE_WIDTH+w_pro;
        A = EpiLine.lineA(h_pro, w_pro);
        B = EpiLine.lineB(h_pro, w_pro);
        M = ParaSet.M(pvec_idx,:);
        projected_x = corres_mats{1,1}{h_pro,w_pro}(1,2);
        projected_y = round(-A/B * projected_x + 1/B);
        epi_corres_mat{h_pro,w_pro} = [projected_y, projected_x];
    end
end
for h = 1:51
    for w = 1:51
        h_c = epi_corres_mat{h,w}(1,1) - CamInfo.cam_range(2,1) + 1;
        w_c = epi_corres_mat{h,w}(1,2) - CamInfo.cam_range(1,1) + 1;
        show_mat(h_c, w_c, :) = 0.0;
        show_mat(h_c, w_c, 2) = 1.0;
    end
end