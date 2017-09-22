function [projected_vec, valid_vec] = fun_ProjectedImage(depth_mat, ...
    pro_color, ...
    pro_sigma, ...
    CamInfo, ...
    ProInfo, ...
    EpiLine, ...
    ParaSet, corres_mat)
% Calculate P

    % parameters set
    projected_vec = zeros(CamInfo.RANGE_HEIGHT*CamInfo.RANGE_WIDTH, 1);
    valid_vec = zeros(CamInfo.RANGE_HEIGHT*CamInfo.RANGE_WIDTH, 1);
    depth_vec = reshape(depth_mat', ...
        ProInfo.RANGE_HEIGHT*ProInfo.RANGE_WIDTH, 1);

    % Fill valid idx
    valid_index = cell(size(projected_vec));
    for h_pro = 1:ProInfo.RANGE_HEIGHT
        for w_pro = 1:ProInfo.RANGE_WIDTH
            k = (h_pro-1)*ProInfo.RANGE_WIDTH+w_pro;
            h_cam = corres_mat{h_pro,w_pro}(1,1);
            w_cam = corres_mat{h_pro,w_pro}(1,2);
            for dlt_h = -4:4
                for dlt_w = -4:4
                    h = round(h_cam) + dlt_h - CamInfo.cam_range(2,1) + 1;
                    w = round(w_cam) + dlt_w - CamInfo.cam_range(1,1) + 1;
                    cam_idx = (h-1)*CamInfo.RANGE_WIDTH + w;
                    valid_index{cam_idx, 1} = [valid_index{cam_idx, 1}; k];
                end
            end
        end
    end
%     for k = 1:ProInfo.RANGE_HEIGHT*ProInfo.RANGE_WIDTH
%         h_pro = ParaSet.coord_pro(k,1);
%         w_pro = ParaSet.coord_pro(k,2);
%         A = EpiLine.lineA(h_pro, w_pro);
%         B = EpiLine.lineB(h_pro, w_pro);
%         projected_x = (ParaSet.M(k,1)*depth_vec(k) + ParaSet.D(k,1)) ...
%             / (ParaSet.M(k,3)*depth_vec(k) + ParaSet.D(k,3));
%         projected_y = - A/B * projected_x + 1/B;
%         for dlt_h = -3:3
%             for dlt_w = -3:3
%                 new_w = round(projected_x) + dlt_w - CamInfo.cam_range(1,1) + 1;
%                 new_h = round(projected_y) + dlt_h - CamInfo.cam_range(2,1) + 1;
%                 cam_idx = (new_h-1)*CamInfo.RANGE_WIDTH + new_w;
%                 valid_index{cam_idx, 1} = [valid_index{cam_idx, 1}; k];
%             end
%         end
%     end

    % Fill projected_image
    for l = 1:CamInfo.RANGE_HEIGHT*CamInfo.RANGE_WIDTH
        if mod(l, 5000) == 0
            fprintf('.');
        end
        projected_vec(l, 1) = 20; % light
        cam_x = ParaSet.coord_cam(l,2) + CamInfo.cam_range(1,1);
        cam_y = ParaSet.coord_cam(l,1) + CamInfo.cam_range(2,1);
        for k_idx = 1:size(valid_index{l, 1}, 1)
            valid_vec(l, 1) = 1;
            k = valid_index{l, 1}(k_idx, 1);
            h_pro = ParaSet.coord_pro(k,1);
            w_pro = ParaSet.coord_pro(k,2);
%             A = EpiLine.lineA(h_pro, w_pro);
%             B = EpiLine.lineB(h_pro, w_pro);
%             projected_x = (ParaSet.M(k,1)*depth_vec(k) + ParaSet.D(k,1)) ...
%                 / (ParaSet.M(k,3)*depth_vec(k) + ParaSet.D(k,3));
%             projected_y = - A/B * projected_x + 1/B;
            projected_x = corres_mat{h_pro, w_pro}(1,2);
            projected_y = corres_mat{h_pro, w_pro}(1,1);
            tmp_exp_val = ((-0.5 / pro_sigma(k,1)^2) ...
                * (cam_x - projected_x)^2 ...
                + ((-0.5 / pro_sigma(k,2)^2) ...
                * (cam_y - projected_y)^2));
            tmp_val = pro_color(k,1) / (2 * pi * pro_sigma(k,1) * pro_sigma(k,2)) ...
                * exp(tmp_exp_val);
            projected_vec(l,1) = projected_vec(l,1) + tmp_val;
        end
    end
    fprintf('\n');
end
