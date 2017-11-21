% Set parameters
warning('off');
clear;
load GeneEpilinePara.mat

% Create lineA, lineB, lineC
EpiLine.lineA = zeros(CamInfo.HEIGHT, CamInfo.WIDTH);
EpiLine.lineB = zeros(size(EpiLine.lineA));
EpiLine.lineC = ones(size(EpiLine.lineA));
xdis_threhold = 10;
xnum_threhold = 5;

% Fill valid points
valid_points = cell(CamInfo.HEIGHT, CamInfo.WIDTH);

for frame_idx = 0:total_frame_num-1
  % fprintf(['F', num2str(frame_idx), ': ']);
  % Load and Intersect
  xpro_mat_0 = load(
      [FilePath.main_file_path, 'cam_0/', FilePath.xpro_file_path, ...
      FilePath.xpro_file_name, num2str(frame_idx), FilePath.pro_file_suffix]);
  ypro_mat_0 = load(
      [FilePath.main_file_path, 'cam_0/', FilePath.ypro_file_path, ...
      FilePath.ypro_file_name, num2str(frame_idx), FilePath.pro_file_suffix]);
  xpro_mat_1 = load(
      [FilePath.main_file_path, 'cam_0/', FilePath.xpro_file_path, ...
      FilePath.xpro_file_name, num2str(frame_idx), FilePath.pro_file_suffix]);
  ypro_mat_1 = load(
      [FilePath.main_file_path, 'cam_1/', FilePath.ypro_file_path, ...
      FilePath.ypro_file_name, num2str(frame_idx), FilePath.pro_file_suffix]);
  % Find Correspondence and refinement
  valid_mat = zeros(CamInfo.HEIGHT, CamInfo.WIDTH);
  for h_0 = 1:CamInfo.HEIGHT
    for w_0 = 1:CamInfo.WIDTH
      x_pro_0 = xpro_mat_0(h_0, w_0);
      y_pro_0 = ypro_mat_0(h_0, w_0);
      if (x_pro_0 < 0) || (y_pro_0 < 0)
        continue;
      end
      found_idx = [0, 0];
      min_val = 1.4;
      for h_1 = 1:CamInfo.HEIGHT
        for w_1 = 1:CamInfo.WIDTH
          x_pro_1 = xpro_mat_1(h_1, w_1);
          y_pro_1 = ypro_mat_1(h_1, w_1);
          if (x_pro_1 < 0) || (y_pro_1 < 0)
            continue;
          end
          cor_dis = (x_pro_1-x_pro_0)^2 + (y_pro_1-y_pro_0)^2;
          if cor_dis < min_val
            min_val = cor_dis;
            found_idx = [w_1, h_1];
          end
        end
      end
      if min_val < 1.4
        valid_points{h_0, w_0} = [valid_points{h_0, w_0}; found_idx];
        valid_mat(h_0, w_0) = 1;
      end
    end
  end
  imshow(valid_mat);
  fprintf('.')
end
fprintf('\n');

fprintf('Calculation...\n');
final_valid_mat = zeros(CamInfo.HEIGHT, CamInfo.WIDTH);
for h_0 = 1:CamInfo.HEIGHT
  for w_0 = 1:CamInfo.WIDTH
    if size(valid_points{h_0, w_0},1) < xnum_threhold
      continue;
    end
    xmax_val = max(valid_points{h_0, w_0}(:,1));
    xmin_val = min(valid_points{h_0, w_0}(:,1));
    if xmax_val - xmin_val > xdis_threhold
      vecY = ones(size(valid_points{h_0,w_0},1),1);
      vecX = valid_points{h_0,w_0} \ vecY;
      final_valid_mat(h_0,w_0) = 1;
      EpiLine.lineA(h_0,w_0) = vecX(1);
      EpiLine.lineB(h_0,w_0) = vecX(2);
    end
  end
end
imshow(final_valid_mat);

save('EpilinePara.mat', 'EpiLine');
