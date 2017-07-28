start_frame_num = 1;
end_frame_num = 17;
half_window_size = 10;
h_qry = 200 + 101 - 1;
w_qry = 600 + 101 - 1;

for frame_idx = start_frame_num+1:end_frame_num
    fprintf('%d -> %d\n', frame_idx-1, frame_idx);

    show_mat = zeros(2*(2*half_window_size+1), 2*half_window_size+1, 3);

    show_mat(1:(2*half_window_size+1), :, 1) ...
        = camera_image{frame_idx-1, ...
        1}(h_qry-half_window_size:h_qry+half_window_size, ...
        w_qry-half_window_size:w_qry+half_window_size);
    show_mat((2*half_window_size+2):2*(2*half_window_size+1), :, 1) ...
        = camera_image{frame_idx, ...
        1}(h_qry-half_window_size:h_qry+half_window_size, ...
        w_qry-half_window_size:w_qry+half_window_size);
    show_mat(:, :, 2) = show_mat(:, :, 1);
    show_mat(:, :, 3) = show_mat(:, :, 1);

    show_mat(half_window_size + 1, half_window_size + 1, 1) = 1.0;
    show_mat(half_window_size + 1, half_window_size + 1, 2) = 0.0;
    show_mat(half_window_size + 1, half_window_size + 1, 3) = 0.0;
    show_mat(3 * half_window_size + 2, half_window_size + 1, 1) = 1.0;
    show_mat(3 * half_window_size + 2, half_window_size + 1, 2) = 0.0;
    show_mat(3 * half_window_size + 2, half_window_size + 1, 3) = 0.0;

    big_show_mat = imresize(show_mat, 10.0, 'nearest');
    imshow(big_show_mat);

end
