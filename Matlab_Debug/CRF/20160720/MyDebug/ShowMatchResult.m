h = 400;
w = 500;
i = 12;

cam_show = camera_image{i}(h-20:h+20, w-20:w+20);
x = xpro_mat(h, w);
y = ypro_mat(h, w);
x = floor(x+0.5);
y = floor(y+0.5);
pro_show = pattern(y-20:y+20, x-20:x+20);

show_mat = [cam_show, pro_show];
imshow(show_mat);