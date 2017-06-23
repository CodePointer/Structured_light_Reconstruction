function alpha = color_alpha(c_xy, p_xy)
    c_xy_color = [0, 0, 0, 0];
    if c_xy < 0.25
        c_xy_color(1) = 1;
    elseif c_xy < 0.5
        c_xy_color(2) = 1;
    elseif c_xy < 0.75
        c_xy_color(3) = 1;
    else
        c_xy_color(4) = 1;
    end

    p_xy_color = [0, 0, 0, 0];
    if p_xy < 0.25
        p_xy_color(1) = 1;
    elseif p_xy < 0.5
        p_xy_color(2) = 1;
    elseif p_xy < 0.75
        p_xy_color(3) = 1;
    else
        p_xy_color(4) = 1;
    end

    difference = norm(c_xy_color - p_xy_color);
    if difference == 0
        alpha = 1;
    else
        alpha = 0.001;
    end
end
