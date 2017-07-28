function psi_t = Psi_t_cal(v_ori, cal_para)
    psi_t = zeros(cal_para.hVoxelRange*2 + 1, 1);
    norm_sigma_a = 4;

    idx_ori = floor((v_ori / cal_para.voxelSize) + 0.5);

    for idx = 1:cal_para.hVoxelRange*2+1
        dVal = abs(idx - idx_ori);
        psi_t(idx, 1) = exp( -dVal^2 / (2*norm_sigma_a) );
    end
end
