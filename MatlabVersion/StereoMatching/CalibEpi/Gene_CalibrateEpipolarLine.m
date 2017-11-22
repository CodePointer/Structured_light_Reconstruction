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
  xpro_mat_0 = load(...
      [FilePath.main_file_path, 'cam_0/', FilePath.xpro_file_path, ...
      FilePath.xpro_file_name, num2str(frame_idx), FilePath.pro_file_suffix]);
  ypro_mat_0 = load(...
      [FilePath.main_file_path, 'cam_0/', FilePath.ypro_file_path, ...
      FilePath.ypro_file_name, num2str(frame_idx), FilePath.pro_file_suffix]);
  xpro_mat_1 = load(...
      [FilePath.main_file_path, 'cam_1/', FilePath.xpro_file_path, ...
      FilePath.xpro_file_name, num2str(frame_idx), FilePath.pro_file_suffix]);
  ypro_mat_1 = load(...
      [FilePath.main_file_path, 'cam_1/', FilePath.ypro_file_path, ...
      FilePath.ypro_file_name, num2str(frame_idx), FilePath.pro_file_suffix]);
  match_mat_h = - ones(ProInfo.HEIGHT, ProInfo.WIDTH);
  match_mat_w = - ones(ProInfo.HEIGHT, ProInfo.WIDTH);
  % Fill match_mat from cam_1
  for h_1 = 1:CamInfo.HEIGHT
    for w_1 = 1:CamInfo.WIDTH
      x_pro_1 = xpro_mat_1(h_1, w_1);
      y_pro_1 = ypro_mat_1(h_1, w_1);
      if (x_pro_1 < 0) || (y_pro_1 < 0)
        continue;
      end
      h_pro = round(y_pro_1);
      w_pro = round(x_pro_1);
      match_mat_h(h_pro, w_pro) = h_1;
      match_mat_w(h_pro, w_pro) = w_1;
    end
  end
  % Find correspondence
  valid_mat = zeros(CamInfo.HEIGHT, CamInfo.WIDTH);
  for h_0 = 1:CamInfo.HEIGHT
    for w_0 = 1:CamInfo.WIDTH
      x_pro_0 = xpro_mat_0(h_0, w_0);
      y_pro_0 = ypro_mat_0(h_0, w_0);
      if (x_pro_0 < 0) || (y_pro_0 < 0)
        continue;
      end
      h_pro = round(y_pro_0);
      w_pro = round(x_pro_0);
      h_1 = match_mat_h(h_pro, w_pro);
      w_1 = match_mat_w(h_pro, w_pro);
      if (h_1 > 0) && (w_1 > 0)
        match_idx = [w_1, h_1];
        valid_points{h_0, w_0} = [valid_points{h_0, w_0}; match_idx];
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
