clear;

% Cam Pro Inner parameters
cameraMatrix = [ 2460.98, 0.0, 639.50;
    0.0, 2462.24, 511.50;
    0.0, 0.0, 1.0];
projectorMatrix = [ 1808.35, 0.0, 787.53;
    0.0, 1813.76, 649.98;
    0.0, 0.0, 1.0];
rotationMatrix = [0.9826, 0.009897, -0.1856;
    -0.02213, 0.9977, -0.06398;
    0.1845, 0.06697, 0.9805];
transportVector = [9.4886;
    -1.6272;
    3.3267];

% Other Set
CAMERA_HEIGHT = 1024;
CAMERA_WIDTH = 1280;
PROJECTOR_HEIGHT = 800;
PROJECTOR_WIDTH = 1280;
main_file_path = 'E:/Structured_Light_Data/20170915/EpipolarCalibration/';
xpro_file_path = 'pro_txt/';
xpro_file_name = 'ipro_mat';
ypro_file_path = 'pro_txt/';
ypro_file_name = 'jpro_mat';
pro_file_suffix = '.txt';
img_file_path = 'dyna/';
img_file_name = 'pattern_3size6color';
img_file_suffix = '.png';
total_frame_num = 20;
pattern = imread([main_file_path, 'pattern_3size6color.png']);
pro_range = [846, 996; 405, 555];

save GeneralPara.mat
