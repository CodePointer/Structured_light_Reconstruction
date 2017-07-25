function [  ] = my_imshow(image, viewportMatrix, norm)
    if norm
        imshow(image(viewportMatrix(2,1):viewportMatrix(2,2), viewportMatrix(1,1):viewportMatrix(1,2)), []);
    else
        imshow(image(viewportMatrix(2,1):viewportMatrix(2,2), viewportMatrix(1,1):viewportMatrix(1,2)));
    end
end

