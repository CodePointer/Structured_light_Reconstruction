% Load parameters
clear;
load EpipolarPara.mat
load GeneralPara.mat

% Read information of 1st frame
cam_mats = cell(total_frame_num, 1);
depth_mats = cell(total_frame_num, 1);
valid_mats = cell(total_frame_num, 1);
corres_mats = cell(total_frame_num, 1);
[depth_mats{1, 1}, corres_mats{1, 1}] = fun_InitDepthMat(FilePath, ProInfo, CamInfo, CalibMat);
tmp_image = double(imread([FilePath.main_file_path, ...
    FilePath.img_file_path, ...
    FilePath.img_file_name, ...
    num2str(11), ...
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

% Prepare for Optimization
now_depth_mat = depth_mats{1, 1};
now_image_mat = cam_mats{2, 1};
color_table = zeros(ProInfo.RANGE_HEIGHT*ProInfo.RANGE_WIDTH, 1);
sigma_table = zeros(ProInfo.RANGE_HEIGHT*ProInfo.RANGE_WIDTH, 1);
% for i = 1:ProInfo.RANGE_HEIGHT*ProInfo.RANGE_WIDTH
%     h_p = ParaSet.coord_pro(i, 1);
%     w_p = ParaSet.coord_pro(i, 2);
%     x_p = ProInfo.pro_range(1,1) + (w_p-1)*3 + 1;
%     y_p = ProInfo.pro_range(2,1) + (h_p-1)*3 + 1;
%     color_index = round((pattern(y_p, x_p)-60)/35);
%     if color_index == 0
%         color_index = 1;
%     end
%     color_table(i, 1) = ParaSet.gauss(color_index, 1);
%     sigma_table(i, 1) = ParaSet.gauss(color_index, 2);
% end
for h = 1:ProInfo.RANGE_HEIGHT
    for w = 1:ProInfo.RANGE_WIDTH
        i = (h-1)*ProInfo.RANGE_WIDTH + w;
        w_c = corres_mats{1, 1}{h, w}(1,1) - CamInfo.cam_range(1,1) + 1;
        h_c = corres_mats{1, 1}{h, w}(1,2) - CamInfo.cam_range(2,1) + 1;
        color_table(i, 1) = cam_mats{1, 1}(h_c, w_c) * 2 * pi * 2^2;
        sigma_table(i, 1) = 2;
    end
end
result_vec = fun_ProjectedImage(now_depth_mat, ...
    color_table, ...
    sigma_table, ...
    CamInfo, ...
    ProInfo, ...
    EpiLine, ...
    ParaSet);
result_mat = reshape(result_vec, CamInfo.RANGE_WIDTH, CamInfo.RANGE_HEIGHT)';
