function [depth_mat, corres_points] = fun_InitDepthMat(FilePath, ProInfo, CamInfo, CalibMat)
% Calculate first depth mat

    % Read xpro_mat, ypro_mat, opt_mat
    xpro_mat = load([FilePath.main_file_path, ...
        FilePath.xpro_file_path, ...
        FilePath.xpro_file_name, ...
        num2str(0), ...
        FilePath.pro_file_suffix]);
    ypro_mat = load([FilePath.main_file_path, ...
        FilePath.ypro_file_path, ...
        FilePath.ypro_file_name, ...
        num2str(0), ...
        FilePath.pro_file_suffix]);
    opt_mat = imread([FilePath.main_file_path, ...
        FilePath.optical_path, ...
        FilePath.optical_name, ...
        FilePath.optical_suffix]);
    depth_mat = zeros(ProInfo.RANGE_HEIGHT, ProInfo.RANGE_WIDTH);

    % Intersect xpro_mat, ypro_mat
    intersect_range = [253, 1037; 210, 746];
    for h = intersect_range(2,1):intersect_range(2,2)
        for w = intersect_range(1,1):intersect_range(1,2)
            if xpro_mat(h,w) < 0
                xpro_mat(h,w) = (xpro_mat(h,w-1) + xpro_mat(h,w+1)) / 2;
            end
            if ypro_mat(h,w) < 0
                ypro_mat(h,w) = (ypro_mat(h-1,w) + ypro_mat(h+1,w)) / 2;
            end
        end
    end

    % Find correspondence point
    corres_points = cell(ProInfo.RANGE_HEIGHT, ProInfo.RANGE_WIDTH);
    for h = intersect_range(2,1):intersect_range(2,2)
        for w = intersect_range(1,1):intersect_range(1,2)
            xpro_int = round(xpro_mat(h,w) + 1);
            ypro_int = round(ypro_mat(h,w) + 1);
            if (xpro_int >= ProInfo.pro_range(1,1)) ...
                && (xpro_int <= ProInfo.pro_range(1,2)) ...
                && (ypro_int >= ProInfo.pro_range(2,1)) ...
                && (ypro_int <= ProInfo.pro_range(2,2))
                if (mod(xpro_int-32, 3) == 2) ...
                    && (mod(ypro_int-8, 3) == 2)
                    h_i = (ypro_int - ProInfo.pro_range(2,1)) / 3 + 1;
                    w_i = (xpro_int - ProInfo.pro_range(1,1)) / 3 + 1;
                    corres_points{h_i, w_i} = [h, w];
                end
            end
        end
    end

    % Use opt_mat to refine depth information
    template_mat = cell(2, 1);
    template_mat{1, 1} = [93, 87, 93; 87, 75, 87; 93, 87, 93;];
    template_mat{2, 1} = [130, 144, 130; 144, 161, 144; 130, 144, 130];
    for h = 1:ProInfo.RANGE_HEIGHT
        for w = 1:ProInfo.RANGE_WIDTH
            xpro_center = (w-1)*3 + ProInfo.pro_range(1,1);
            ypro_center = (h-1)*3 + ProInfo.pro_range(2,1);
            t_idx = mod((xpro_center+ypro_center-32-8-4)/3, 2) + 1;
            match_res = zeros(3, 3);
            for h_s = -1:1
                for w_s = -1:1
                    h_center = corres_points{h, w}(1,1) + h_s;
                    w_center = corres_points{h, w}(1,2) + w_s;
                    opt_part = double(opt_mat(h_center-1:h_center+1, ...
                        w_center-1:w_center+1));
                    match_res(h_s+2,w_s+2) = sum(sum((template_mat{t_idx,1} - opt_part).^2));
                end
            end
            [h_min, w_min] = find(match_res == min(min(match_res)));
            corres_points{h,w} = corres_points{h,w} + [h_min-2, w_min-2];
        end
    end

    % Calculate depth mat
    proMat = CalibMat.proMat;
    camMat = CalibMat.camMat;
    A = proMat(1,1) * proMat(2,2) * camMat(1,4);
    B = proMat(1,1) * proMat(2,2) * camMat(3,4);
    for h = 1:ProInfo.RANGE_HEIGHT
        for w = 1:ProInfo.RANGE_WIDTH
            x_p = w + ProInfo.pro_range(1,1) - 1;
            y_p = h + ProInfo.pro_range(2,1) - 1;
            x_c = corres_points{h,w}(1,2);
            C = (x_p - proMat(1,3)) * proMat(2,2) * camMat(1,1) ...
                + (y_p - proMat(2,3)) * proMat(1,1) * camMat(1,2) ...
                + proMat(1,1) * proMat(2,2) * camMat(1,3);
            D = (x_p - proMat(1,3)) * proMat(2,2) * camMat(3,1) ...
                + (y_p - proMat(2,3)) * proMat(1,1) * camMat(3,2) ...
                + proMat(1,1) * proMat(2,2) * camMat(3,3);
            depth_mat(h, w) = - (A-B*x_c) / (C-D*x_c);
        end
    end
end
