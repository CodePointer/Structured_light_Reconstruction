clear;

% CamInfo set
CamInfo.HEIGHT = 1024;
CamInfo.WIDTH = 1280;
CamInfo.cam_range = [571, 838; 443, 699];
CamInfo.RANGE_HEIGHT = CamInfo.cam_range(2,2) - CamInfo.cam_range(2,1) + 1;
CamInfo.RANGE_WIDTH = CamInfo.cam_range(1,2) - CamInfo.cam_range(1,1) + 1;
CamInfo.inter_range = [128, 1092; 252, 768];

% ProInfo set
ProInfo.HEIGHT = 800;
ProInfo.WIDTH = 1280;
ProInfo.RANGE_HEIGHT = 51;
ProInfo.RANGE_WIDTH = 51;
ProInfo.pro_range = [769, 919; 445, 595]; % matlab coordinates

% FilePath
FilePath.main_file_path = 'E:/Structured_Light_Data/20171008/EpiLineCalib/';
FilePath.optical_path = 'pro/';
FilePath.optical_name = 'pattern_optflow';
FilePath.optical_suffix = '.png';
FilePath.xpro_file_path = 'pro_txt/';
FilePath.xpro_file_name = 'xpro_mat';
FilePath.ypro_file_path = 'pro_txt/';
FilePath.ypro_file_name = 'ypro_mat';
FilePath.pro_file_suffix = '.txt';
FilePath.img_file_path = 'dyna/';
% FilePath.img_file_name = 'pattern_3size6color';
FilePath.img_file_name = 'dyna_mat';
FilePath.img_file_suffix = '.png';

total_frame_num = 20;

save('GeneralParaEpi.mat', ...
    'CamInfo', 'ProInfo', 'FilePath', 'total_frame_num');
