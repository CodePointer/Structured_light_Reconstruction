function debugShowDepthFieldOnPoint(beliefField, x, y)
    tmp_vec = -0.8:0.02:0.8;
    x_value = reshape(tmp_vec, [81, 1]);
    y_value = reshape(beliefField{y, x}, [81, 1]);
    figure, plot(x_value, y_value, '.', 'MarkerSize', 20);
end
