function [cam_vecs] = fun_ReadCamVecFromFile(FilePath, ...
    CamInfo, ...
    total_frame_num)
% Read camera image and reshape to vecs

    cam_vecs = cell(total_frame_num,1);
    for frm_idx = 1:total_frame_num
        tmp_img = double(imread([FilePath.main_file_path, ...
            FilePath.img_file_path, ...
            FilePath.img_file_name, ...
            num2str(frm_idx), ...
            FilePath.img_file_suffix]));
        range_img = tmp_img(CamInfo.cam_range(2,1):CamInfo.cam_range(2,2), ...
            CamInfo.cam_range(1,1):CamInfo.cam_range(1,2));
        cam_vecs{frm_idx,1} = reshape(range_img', ...
            CamInfo.RANGE_HEIGHT*CamInfo.RANGE_WIDTH, 1);
    end

end
