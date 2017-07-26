delta_depth_mat = tmp_delta_depth_mat;
last_camera_mat = camera_image{frame_idx-1, 1};
now_camera_mat = camera_image{frame_idx, 1};

color_mat = zeros(size(last_camera_mat));
for h = viewportMatrix(2,1):viewportMatrix(2,2)
    for w = viewportMatrix(1,1):viewportMatrix(1,2)
        if abs(now_camera_mat(h, w) - 0.5) < 0.2
            color_mat(h, w) = 1;
        end
    end
end


tmp_show_mat = zeros(size(last_camera_mat));
for h = viewportMatrix(2,1):viewportMatrix(2,2)
    for w = viewportMatrix(1,1):viewportMatrix(1,2)
        if abs(now_camera_mat(h, w) - 0.5) > 0.2
            tmp_show_mat(h, w) = tmp_delta_depth_mat(h, w);
        end
    end
end
figure, my_imshow(tmp_show_mat, viewportMatrix, true);