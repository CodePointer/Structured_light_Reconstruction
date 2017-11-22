clear;

% Cam Pro Inner parameters
CalibMat.cam_0 = [ 2432.058972474525, 0, 762.2933947666461;
  0, 2435.900798664577, 353.2790048217345;
  0, 0, 1];
CalibMat.cam_1 = [ 2428.270501026523, 0, 717.1879617522386;
  0, 2425.524847530806, 419.6450731465209;
  0, 0, 1];
CalibMat.rot = [0.9991682873520409, 0.01604901003987891, 0.03748550155365887;
  -0.01624095229582852, 0.9998564818205395, 0.004821538134006965;
  -0.0374027407887994, -0.00542632824227644, 0.9992855397449185];
CalibMat.trans = [-4.672867184359712;
  0.08985783911144951;
  -1.53686618071908];
CalibMat.cam_0_mat = [CalibMat.cam_0, zeros(3,1)];
CalibMat.cam_1_mat = CalibMat.cam_1 * [CalibMat.rot, CalibMat.trans];

% CamInfo set
CamInfo.HEIGHT = 1024;
CamInfo.WIDTH = 1280;
% CamInfo.range_mat = [434, 825+40; 463, 809];
CamInfo.range_mat = [0, CamInfo.WIDTH; 0, CamInfo.HEIGHT];
CamInfo.R_HEIGHT = CamInfo.range_mat(2,2) - CamInfo.range_mat(2,1) + 1;
CamInfo.R_WIDTH = CamInfo.range_mat(1,2) - CamInfo.range_mat(1,1) + 1;
CamInfo.coord_trans = zeros(CamInfo.R_HEIGHT*CamInfo.R_WIDTH, 2);
for h = 1:CamInfo.R_HEIGHT
    for w = 1:CamInfo.R_WIDTH
        CamInfo.coord_trans((h-1)*CamInfo.R_WIDTH + w, :) = [h,w];
    end
end
CamInfo.win_rad = 9;

% FilePath
FilePath.main_file_path = 'E:/Structured_Light_Data/20171121/Plane_MR/';
FilePath.xpro_file_path = 'pro/';
FilePath.xpro_file_name = 'xpro_mat';
FilePath.ypro_file_path = 'pro/';
FilePath.ypro_file_name = 'ypro_mat';
FilePath.pro_file_suffix = '.txt';
FilePath.img_file_path = 'dyna/';
FilePath.img_file_name = 'dyna_mat';
FilePath.img_file_suffix = '.png';
FilePath.output_file_path = 'result/';
FilePath.output_file_name = 'pc';
FilePath.output_file_suffix = '.txt';
pattern = imread([FilePath.main_file_path, 'pattern_2size4color0.png']);

% Other ParaSet
ParaSet.M = zeros(CamInfo.R_HEIGHT*CamInfo.R_WIDTH, 3);
ParaSet.D = zeros(CamInfo.R_HEIGHT*CamInfo.R_WIDTH, 3);
for h_cam = 1:CamInfo.R_HEIGHT
    for w_cam = 1:CamInfo.R_WIDTH
        cvec_idx = (h_cam-1)*CamInfo.R_WIDTH + w_cam;
        x_cam = w_cam + CamInfo.range_mat(1,1) - 1 - 1;
        y_cam = h_cam + CamInfo.range_mat(2,1) - 1 - 1;
        tmp_vec = [(x_cam - CalibMat.cam_0(1,3)) / CalibMat.cam_0(1,1); ...
            (y_cam - CalibMat.cam_0(2,3)) / CalibMat.cam_0(2,2); ...
            1];
        ParaSet.M(cvec_idx,:) = (CalibMat.cam_1_mat(:,1:3)*tmp_vec)';
        ParaSet.D(cvec_idx,:) = CalibMat.cam_1_mat(:,4)';
    end
end
ParaSet.lumi_thred = 30;

total_frame_num = 40;

save('GeneralPara.mat', ...
    'CalibMat', 'CamInfo', ...
    'FilePath', 'ParaSet', 'total_frame_num', 'pattern');
