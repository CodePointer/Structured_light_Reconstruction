clear;

part_PreProcess;

for h_0 = CamInfo.win_rad+1:CamInfo.HEIGHT-CamInfo.win_rad
  for w_0 = CamInfo.win_rad+1:CamInfo.WIDTH-CamInfo.win_rad
    if (EpiLine.lineA(h_0,w_0) == 0) && (EpiLine.lineB(h_0,w_0) == 0)
      continue;
    end
    patch_0 = cam_mat_0(...
        h_0 - CamInfo.win_rad:h_0 + CamInfo.win_rad, ...
        w_0 - CamInfo.win_rad:w_0 + CamInfo.win_rad);
    % check patch_0 is valid or not
    max_0 = max(max(patch_0));
    min_0 = min(min(patch_0));
    if (max_0 - min_0 < ParaSet.lumi_thred)
      continue;
    end

    mask_mat(h_0,w_0) = 1;
    cvec_idx = (h_0-1)*CamInfo.WIDTH + w_0;
    M = ParaSet.M(cvec_idx,:);
    D = ParaSet.D(cvec_idx,:);
    A = EpiLine.lineA(h_0, w_0);
    B = EpiLine.lineB(h_0, w_0);
    min_val = 1e15;
    min_h_idx = 0;
    min_w_idx = 0;
    min_depth = 0;
    % epi_show = cam_mat_1;
    for w_1 = CamInfo.win_rad+1:CamInfo.WIDTH-CamInfo.win_rad
      depth = - (D(1)-D(3)*w_1) / (M(1)-M(3)*w_1);
      h_center = round(-A/B * w_1 + 1/B);
      if (h_center <= 0) || (h_center > CamInfo.HEIGHT)
        continue;
      end
      % epi_show(h_center,w_1) = 255;
      for h_1 = h_center-2:h_center+2
        if (h_1 >= CamInfo.win_rad+1) && (h_1 <= CamInfo.HEIGHT-CamInfo.win_rad)
          patch_1 = cam_mat_1(...
              h_1 - CamInfo.win_rad:h_1 + CamInfo.win_rad, ...
              w_1 - CamInfo.win_rad:w_1 + CamInfo.win_rad);
          max_1 = max_val_cam_1(h_1, w_1);
          min_1 = min_val_cam_1(h_1, w_1);
          if (max_1 - min_1 < ParaSet.lumi_thred)
              continue;
          end
          error_value = sum(sum((patch_0 - patch_1).^2));
          if error_value < min_val
            min_val = error_value;
            min_h_idx = h_1;
            min_w_idx = w_1;
            min_depth = depth;
          end
        end
      end
    end
    depth_mat(h_0, w_0) = min_depth;
    match_h_mat(h_0, w_0) = min_h_idx;
    match_w_mat(h_0, w_0) = min_w_idx;
    error_mat(h_0, w_0) = min_val;
  end
  fprintf('.');
  if mod(h_0, 20) == 0
    imshow(depth_mat,[30,50]);
    pause(0.1);
  end
  if mod(h_0, 80) == 0
    fprintf('\n');  
  end
end
fprintf('\n');
% output
% save status.mat
