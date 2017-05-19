function dCheckCorrespondence(Img_c, pos_c, Pattern)
    show_Img_c = Img_c(pos_c(1)-10:pos_c(1)+10, pos_c(2)-10:pos_c(2)+10);
    
    % TODO(pointer): Finish pos_c to pos_p with parameters
    pos_p = pos_c;
    
    show_Pattern =Pattern(pos_p(1)-10:pos_p(1)+10, pos_p(2)-10:pos_p(2)+10);
    
    figure(1)
    imshow([show_Img_c;show_Pattern]);
end