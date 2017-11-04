img_path = 'E:/Structured_Light_Data/20171103/SphereMovement_part/dyna/';
pc_path = './pc/';
output_name = './pc/output ';
pc_range = [334,666;112,359;];
total_frame = 30;
image_size = [400, 300];

for frm_idx = 1:total_frame
    pc_img = imread([pc_path, 'pc (', num2str(frm_idx), ').png']);
    cam_img = imread([img_path, 'dyna_mat', num2str(frm_idx+20), '.png']);
    pc_part_img = pc_img(pc_range(2,1):pc_range(2,2), ...
        pc_range(1,1):pc_range(1,2));
    rs_cam = imresize(cam_img, [400, 600]);
    rs_pc = imresize(pc_part_img, [400,400]);
    imwrite([rs_cam, rs_pc], [output_name, num2str(frm_idx), '.png']);
end
