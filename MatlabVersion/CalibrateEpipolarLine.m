% Set parameters
warning('off');
clear;
load GeneralPara.mat
total_frame_num = 20;

% Create lineA, lineB, lineC
lineA = zeros(PROJECTOR_RANGE_HEIGHT, PROJECTOR_RANGE_WIDTH);
lineB = zeros(size(lineA));
lineC = ones(size(lineA));
xdis_threhold = 10;

% Fill valid points
valid_points = cell(PROJECTOR_RANGE_HEIGHT, PROJECTOR_RANGE_WIDTH);
intersect_range = [253, 1037; 210, 746];
template_mat = cell(2, 1);
template_mat{1, 1} = [93, 87, 93; 87, 75, 87; 93, 87, 93;];
template_mat{2, 1} = [130, 144, 130; 144, 161, 144; 130, 144, 130];

for frame_idx = 0:20
    fprintf('F0: ');
    
    % Load and Intersect
    xpro_mat = load([main_file_path, xpro_file_path, ...
        xpro_file_name, num2str(frame_idx), pro_file_suffix]);
    ypro_mat = load([main_file_path, ypro_file_path, ...
        ypro_file_name, num2str(frame_idx), pro_file_suffix]);
    opt_mat = imread([main_file_path, img_file_path, ...
        'pattern_optflow', num2str(frame_idx), img_file_suffix]);
    img_mat = imread([main_file_path, img_file_path, ...
        img_file_name, num2str(frame_idx), img_file_suffix]);
    for h = intersect_range(2, 1):intersect_range(2, 2)
        for w = intersect_range(1, 1):intersect_range(1, 2)
            if xpro_mat(h, w) < 0
                xpro_mat(h, w) = (xpro_mat(h, w-1) + xpro_mat(h, w+1)) / 2;
            end
            if ypro_mat(h, w) < 0
                ypro_mat(h, w) = (ypro_mat(h-1, w) + ypro_mat(h+1, w)) / 2;
            end
        end
    end
    fprintf('L; ');
    
    % Find Correspondence and refinement
    tmp_valid_points = cell(PROJECTOR_RANGE_HEIGHT, PROJECTOR_RANGE_WIDTH);
    for h = intersect_range(2, 1):intersect_range(2, 2)
        for w = intersect_range(1, 1):intersect_range(1, 2)
            xpro_int = round(xpro_mat(h, w) + 1);
            ypro_int = round(ypro_mat(h, w) + 1);
            if (xpro_int < pro_range(1, 1)) || (xpro_int > pro_range(1, 2)) ...
                || (ypro_int < pro_range(2, 1)) || (ypro_int > pro_range(2, 2))
                continue;
            end
            if (mod(xpro_int-32, 3) == 2) && (mod(ypro_int-8, 3) == 2)
                h_i = (ypro_int - pro_range(2, 1)) / 3 + 1;
                w_i = (xpro_int - pro_range(1, 1)) / 3 + 1;
                tmp_valid_points{h_i, w_i} = [h, w];
            end
        end
    end
    % Use optical flow for refinement
    for h = 1:PROJECTOR_RANGE_HEIGHT
        for w = 1:PROJECTOR_RANGE_WIDTH
            xpro_center = (w-1)*3 + pro_range(1, 1);
            ypro_center = (h-1)*3 + pro_range(2, 1);
            tpl_idx = mod((xpro_center+ypro_center-32-8-4)/3, 2) + 1;
            match_res = zeros(5, 5);
            for h_s = -2:2
                for w_s = -2:2
                    h_center = tmp_valid_points{h, w}(1, 1) + h_s;
                    w_center = tmp_valid_points{h, w}(1, 2) + w_s;
                    img_part = double(opt_mat(h_center-1:h_center+1, w_center-1:w_center+1));
                    match_res(h_s+3, w_s+3) = sum(sum((template_mat{tpl_idx, 1} - img_part).^2));
                end
            end
            [h_res, w_res] = find(min(min(match_res)));
            valid_points{h, w} = [valid_points{h, w}; ...
                [tmp_valid_points{h, w}(1, 1) + h_res - 3, ...
                tmp_valid_points{h, w}(1, 2) + w_res - 3]];
        end
    end
    fprintf('O; ');
    fprintf('.')
end
fprintf('\n');

% Fill valid mat according to valid_points
valid_mat = zeros(CAMERA_HEIGHT, CAMERA_WIDTH);
for h = 1:CAMERA_HEIGHT
    for w = 1:CAMERA_WIDTH
        if size(valid_points{h, w}, 1) == 0
            continue;
        end
        xmax_val = max(valid_points{h, w}(:, 1));
        xmin_val = min(valid_points{h, w}(:, 1));
        if xmax_val - xmin_val > xdis_threhold
            vecX = valid_points{h, w} \ ones(size(valid_points{h, w}, 1), 1);
            valid_mat(h, w) = 1;
            lineA(h, w) = vecX(1);
            lineB(h, w) = vecX(2);
        end
    end
end

save('EpipolarPara.mat', 'lineA', 'lineB', 'lineC');
