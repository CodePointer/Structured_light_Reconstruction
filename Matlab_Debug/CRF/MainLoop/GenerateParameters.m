clear;

%% Inner Parameters
% cameraMatrix = [ 2400.0, 0.0, 640.0;
%     0.0, 2400.0, 512.0;
%     0.0, 0.0, 1.0];
% projectorMatrix = [ 2400.0, 0.0, 640.0;
%     0.0, 2400.0, 400.0;
%     0.0, 0.0, 1.0];
% rotationMatrix = [1.0, 0.0, 0.0;
%     0.0, 1.0, 0.0;
%     0.0, 0.0, 1.0];
% transportVector = [5.0;
%     0.0;
%     0.0];

cameraMatrix = [ 2321.35, 0.0, 639.50;
    0.0, 2326.22, 511.50;
    0.0, 0.0, 1.0];
projectorMatrix = [ 1986.02, 0.0, 598.16;
    0.0, 1983.35, 856.85;
    0.0, 0.0, 1.0];
rotationMatrix = [1.0, 0.0, 0.0;
    0.0, 1.0, 0.0;
    0.0, 0.0, 1.0];
transportVector = [5.0;
    0.0;
    0.0];

CAMERA_HEIGHT = 1024;
CAMERA_WIDTH = 1280;
PROJECTOR_HEIGHT = 800;
PROJECTOR_WIDTH = 1280;
viewportMatrix = [200, 400;
    400, 600];

main_file_path = 'E:/Structured_Light_Data/20170610/1/';
depth_file_path = 'depth_gd/';
depth_file_name = 'depth';
pro_x_file_path = 'pro_x_gd/';
pro_x_file_name = 'pro_x';
pro_y_file_path = 'pro_y_gd/';
pro_y_file_name = 'pro_y';
cam_img_file_path = 'camera_img/';
cam_img_file_name = 'camera_img';
file_suffix = '.txt';
cam_file_suffix = '.png';
data_frame_num = 20;
pattern = double(imread([main_file_path, '4RandDot0.png'])) / 255.0;

% For calculation
norm_sigma = 3;
voxelSize = 0.1;
halfVoxelRange = 20;
save parameters.mat
