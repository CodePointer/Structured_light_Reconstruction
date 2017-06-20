function y_p = xpro2ypro(x_c, y_c, x_p, lineA, lineB, lineC)
    paraA = lineA(x_c, y_c);
    paraB = lineB(x_c, y_c);
    paraC = lineC(x_c, y_c);

    y_p = - (paraA / paraB) * x_p - (paraC / paraB);
end

