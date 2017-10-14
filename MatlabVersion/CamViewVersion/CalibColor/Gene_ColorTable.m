% Set parameters
warning('off');
clear;
load ParaEpi.mat
load EpipolarPara.mat

% Create projector tables
ParaTable.color = zeros(ProInfo.RANGE_HEIGHT*ProInfo.RANGE_WIDTH,1);
ParaTable.sigma = ones(ProInfo.RANGE_HEIGHT*ProInfo.RANGE_WIDTH,2);

% Set ParaTable.sigma:
ParaTable.sigma = ParaTable.sigma * 2;

% Set ParaTable.color:
cam_vecs = cell(total_frame_num, 1);
depth_vecs = cell(total_frame_num, 1);
for frm_idx = 0:total_frame_num-1
    % Read datas
    cam_mat;
    xpro_mat = load([FilePath.main_file_path, ...
        FilePath.xpro_file_path, ...
        FilePath.xpro_file_name, ...
        num2str(frm_idx), ...
        FilePath.pro_file_suffix]);
    ypro_mat = load([FilePath.main_file_path, ...
        FilePath.ypro_file_path, ...
        FilePath.ypro_file_name, ...
        num2str(frame_idx), ...
        FilePath.pro_file_suffix]);

    % Intersect


end
