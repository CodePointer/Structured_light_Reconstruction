load GeneralPara.mat
load EpilinePara.mat

% Set initial frame info
fprintf('Initial...');
cam_mat_0 = double(imread([FilePath.main_file_path, ...
    'cam_0/', ...
    FilePath.img_file_path, ...
    FilePath.img_file_name, ...
    num2str(5), ...
    FilePath.img_file_suffix]));
cam_mat_1 = double(imread([FilePath.main_file_path, ...
    'cam_1/', ...
    FilePath.img_file_path, ...
    FilePath.img_file_name, ...
    num2str(5), ...
    FilePath.img_file_suffix]));
depth_mat = zeros(CamInfo.HEIGHT, CamInfo.WIDTH);
match_w_mat = zeros(CamInfo.HEIGHT, CamInfo.WIDTH);
match_h_mat = zeros(CamInfo.HEIGHT, CamInfo.WIDTH);
mask_mat = zeros(CamInfo.HEIGHT, CamInfo.WIDTH);
error_mat = zeros(CamInfo.HEIGHT, CamInfo.WIDTH);
max_val_cam_1 = zeros(CamInfo.HEIGHT, CamInfo.WIDTH);
min_val_cam_1 = ones(CamInfo.HEIGHT, CamInfo.WIDTH) * 255;
for h_1 = CamInfo.win_rad+1:CamInfo.HEIGHT-CamInfo.win_rad
  for w_1 = CamInfo.win_rad+1:CamInfo.WIDTH-CamInfo.win_rad
    patch_1 = cam_mat_1(h_1-CamInfo.win_rad:h_1+CamInfo.win_rad, ...
                        w_1-CamInfo.win_rad:w_1+CamInfo.win_rad);
    max_val_cam_1(h_1, w_1) = max(max(patch_1));
    min_val_cam_1(h_1, w_1) = min(min(patch_1));
  end
end

fprintf('finished.\n');