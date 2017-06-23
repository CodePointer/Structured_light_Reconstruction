function show_mat = WritePatternColor(cameraImage, ...
    pattern, ...
    xpro_mat, ...
    lineA, ...
    lineB, ...
    lineC)

    [CAMERA_HEIGHT, CAMERA_WIDTH] = size(cameraImage);
    show_mat = ones(size(cameraImage));

    for h = 1:CAMERA_HEIGHT
        for w = 1:CAMERA_WIDTH
            if xpro_mat(h, w) > 0
                xpro = xpro_mat(h, w);
                ypro = xpro2ypro(w, h, xpro, lineA, lineB, lineC);
                show_mat(h, w) = pattern(floor(ypro), floor(xpro));
            end
        end
    end

end
