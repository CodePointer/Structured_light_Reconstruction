hBegin = 450;
hEnd = 650;
wBegin = 700;
wEnd = 900;
Img0 = imread('./dyna/dyna_mat0.png');

fix_h0 = 502;
fix_w0 = 855;

for frameIdx = 2:40
    iH = load(['./result/trace_m/iH', num2str(frameIdx), '.txt']);
    iW = load(['./result/trace_m/iW', num2str(frameIdx), '.txt']);
    Img = imread(['./dyna/dyna_mat', num2str(frameIdx), '.png']);
    
    rgbImg = zeros(1024, 1280, 3);
    rgbImg(:,:,1) = double(Img) / 255.0;
    rgbImg(:,:,2) = double(Img) / 255.0;
    rgbImg(:,:,3) = double(Img) / 255.0;
    
    i = fix_h0 - hBegin;
    j = fix_w0 - wBegin;
    h = iH(i, j) + 1;
    w = iW(i, j) + 1;
    
    rgbImg(h-10:h+10, w-10:w+10, 1) = 1.0;
    rgbImg(h-10:h+10, w-10:w+10, 2) = 0.0;
    rgbImg(h-10:h+10, w-10:w+10, 3) = 0.0;
    
    winSize = 10;
    h0 = i + hBegin;
    w0 = j + wBegin;
    
    patchImg0_origin = Img0(h0-winSize:h0+winSize, w0-winSize:w0+winSize);
    min_val = double(min(min(patchImg0_origin)));
    max_val = double(max(max(patchImg0_origin)));
    patchImg0 = (double(patchImg0_origin) - min_val) / (max_val - min_val);
    
    patchImgi_origin = Img(h-winSize:h+winSize, w-winSize:w+winSize);
    min_val = double(min(min(patchImgi_origin)));
    max_val = double(max(max(patchImgi_origin)));
    patchImgi = (double(patchImgi_origin) - min_val) / (max_val - min_val);
    
    ErrorPixel = (patchImg0 - patchImgi) * 255;
    fprintf('Error: %.2f\n', sum(sum(ErrorPixel.^2))/441);
    patchShow = imresize([patchImg0;patchImgi], 4);
    figure(1),imshow(patchShow);
    
%     for h = hBegin:hEnd-1
%         for w = wBegin:wEnd-1
%             Img(h, w) = 255;
%         end
%     end
    
    figure(2),imshow(rgbImg);
    
    fprintf('%d frame.\n', frameIdx);
    
end