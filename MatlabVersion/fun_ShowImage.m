function [] = fun_ShowImage(projected_vec, CamInfo)
% Show image from vector

    cam_mat = reshape(projected_vec, ...
        CamInfo.RANGE_WIDTH, CamInfo.RANGE_HEIGHT)';
    imshow(cam_mat, []);
end
