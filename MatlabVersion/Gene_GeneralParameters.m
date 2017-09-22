clear;

% Cam Pro Inner parameters
CalibMat.cam = [ 2460.98, 0.0, 639.50;
    0.0, 2462.24, 511.50;
    0.0, 0.0, 1.0];
CalibMat.pro = [ 1808.35, 0.0, 787.53;
    0.0, 1813.76, 649.98;
    0.0, 0.0, 1.0];
CalibMat.rot = [0.9826, 0.009897, -0.1856;
    -0.02213, 0.9977, -0.06398;
    0.1845, 0.06697, 0.9805];
CalibMat.trans = [9.4886;
    -1.6272;
    3.3267];
CalibMat.proMat = [CalibMat.pro, zeros(3, 1)];
CalibMat.camMat = CalibMat.cam * [inv(CalibMat.rot), -CalibMat.trans];

% Other Set
CamInfo.HEIGHT = 1024;
CamInfo.WIDTH = 1280;
CamInfo.RANGE_HEIGHT = 331;
CamInfo.RANGE_WIDTH = 401;
CamInfo.cam_range = [400, 800; 390, 720];
ProInfo.HEIGHT = 800;
ProInfo.WIDTH = 1280;
ProInfo.RANGE_HEIGHT = 51;
ProInfo.RANGE_WIDTH = 51;
ProInfo.pro_range = [769, 919; 445, 595]; % matlab coordinates
FilePath.main_file_path = 'E:/Structured_Light_Data/20170919/SquareMovement/';
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
pattern = imread([FilePath.main_file_path, 'pattern_3size6color.png']);

ParaSet.coord_cam = zeros(CamInfo.RANGE_HEIGHT*CamInfo.RANGE_WIDTH, 2);
for h = 1:CamInfo.RANGE_HEIGHT
    for w = 1:CamInfo.RANGE_WIDTH
        ParaSet.coord_cam((h-1)*CamInfo.RANGE_WIDTH + w, :) = [h, w];
    end
end
ParaSet.coord_pro = zeros(ProInfo.RANGE_HEIGHT*ProInfo.RANGE_WIDTH, 2);
for h = 1:ProInfo.RANGE_HEIGHT
    for w = 1:ProInfo.RANGE_WIDTH
        ParaSet.coord_pro((h-1)*ProInfo.RANGE_WIDTH + w, :) = [h, w];
    end
end
ParaSet.M = zeros(ProInfo.RANGE_HEIGHT*ProInfo.RANGE_WIDTH, 3);
ParaSet.D = zeros(ProInfo.RANGE_HEIGHT*ProInfo.RANGE_WIDTH, 3);
for i = 1:ProInfo.RANGE_HEIGHT*ProInfo.RANGE_WIDTH
    x_p = (ParaSet.coord_pro(i,2)-1)*3 + ProInfo.pro_range(1,1);
    y_p = (ParaSet.coord_pro(i,1)-1)*3 + ProInfo.pro_range(2,1);
    tmp_vec = [(x_p - CalibMat.pro(1,3))/CalibMat.pro(1,1); ...
        (y_p - CalibMat.pro(2,3))/CalibMat.pro(2,2); ...
        1];
    ParaSet.M(i,:) = (CalibMat.camMat(:, 1:3)*tmp_vec)';
    ParaSet.D(i,:) = CalibMat.camMat(:, 4)';
end
ParaSet.gauss = [0,1,1;
    435.17, 1.471, 1.471;
    709.79, 1.329, 1.329;
    1043.29, 1.308, 1.308;
    1382.99, 1.254, 1.254;
    1728.74, 1.254, 1.254] * 1;
ParaSet.gauss(:, 1) = ParaSet.gauss(:, 1) * 1;

save('GeneralPara.mat', ...
    'CalibMat', 'CamInfo', 'ProInfo', ...
    'FilePath', 'ParaSet', 'total_frame_num', 'pattern');