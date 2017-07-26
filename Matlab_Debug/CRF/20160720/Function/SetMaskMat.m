function [ mask_mat ] = SetMaskMat( last_camera_image, now_camera_image, viewportMatrix )
    mask_mat = zeros(size(last_camera_image));
    threhold = 0.2;
    
    for h = viewportMatrix(2,1):viewportMatrix(2,2)
        for w = viewportMatrix(1,1):viewportMatrix(1,2)
            if abs(now_camera_image(h, w) - 0.5) > threhold
                mask_mat(h, w) = 1;
            end
        end
    end

end

