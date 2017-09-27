% load GeneralPara.mat

% x_pro_mat = load('E:/Structured_Light_Data/20170927/wqy/pro_txt/xpro_mat0.txt');

ProMat = CalibMat.pro * [CalibMat.rot, CalibMat.trans];
depth_mat = ones(1024, 1280)*1000;
% mask = zeros(1024, 1280);
% mask(depth_mat < 0) = 1;

fid = fopen('wqy.txt', 'wt+');
for x_cam = 1:1280
    for y_cam = 1:1024
        x_pro = x_pro_mat(y_cam, x_cam);
        if (x_pro < 0)
            continue;
        end
        tmp_vec = [(x_cam - CalibMat.cam(1,3)) / CalibMat.cam(1,1);
            (y_cam - CalibMat.cam(2,3)) / CalibMat.cam(2,2);
            1];
        M = ProMat(:,1:3) * tmp_vec;
        D = ProMat(:,4);
        
        z_wrd = - (D(1) - D(3)*x_pro) / (M(1) - M(3)*x_pro);
        x_wrd = (x_cam - CalibMat.cam(1,3)) / CalibMat.cam(1,1) * z_wrd;
        y_wrd = (y_cam - CalibMat.cam(2,3)) / CalibMat.cam(2,2) * z_wrd;
        depth_mat(y_cam, x_cam) = z_wrd;
        
        fprintf(fid, '%.2f, %.2f, %.2f\n', x_wrd, y_wrd, z_wrd);
    end
end
fclose(fid);

for h = 1:1024
    for w = 2:1279
        if depth_mat(h, w) > 990
            if depth_mat(h, w-1) < 990 && depth_mat(h, w+1) < 990
                depth_mat(h, w) = (depth_mat(h,w-1)+depth_mat(h,w+1))/2;
            end
        end
    end
end

% depth_mat(depth_mat < 0) = 1000;
imshow(-depth_mat, [-303, -73])