% Set parameters
warning('off');
main_file_path = 'E:/Structured_Light_Data/20170915/EpipolarCalibration/';
xpro_file_path = 'pro_txt/';
xpro_file_name = 'ipro_mat';
ypro_file_path = 'pro_txt/';
ypro_file_name = 'jpro_mat';
file_suffix = '.txt';
CAMERA_HEIGHT = 1024;
CAMERA_WIDTH = 1280;

% Create lineA, lineB, lineC
lineA = zeros(CAMERA_HEIGHT, CAMERA_WIDTH);
lineB = zeros(CAMERA_HEIGHT, CAMERA_WIDTH);
lineC = ones(CAMERA_HEIGHT, CAMERA_WIDTH);
xdis_threhold = 10;

% Fill valid points
valid_points = cell(CAMERA_HEIGHT, CAMERA_WIDTH);
for frame_idx = 0:20
    xpro_mat = load([main_file_path, xpro_file_path, ...
        xpro_file_name, num2str(frame_idx), file_suffix]);
    ypro_mat = load([main_file_path, ypro_file_path, ...
        ypro_file_name, num2str(frame_idx), file_suffix]);

    for h = 1:CAMERA_HEIGHT
        for w = 1:CAMERA_WIDTH
            if (xpro_mat(h, w) > 0) && (ypro_mat(h, w) > 0)
                valid_points{h, w} = [valid_points{h, w}; ...
                    [xpro_mat(h, w), ypro_mat(h, w)]];
            end
        end
    end
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
