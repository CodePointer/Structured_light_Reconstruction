function norm_derv_vec = fun_CoarseErrorFunctionDerv(cam_est_vec, ...
    cam_obs_vec, ...
    last_cam_est_vec, ...
    last_cam_obs_vec, ...
    cam_est_derv, ...
    d_depth_coarse_vec, ...
    theta, ...
    ParaSet)

    % Data term
    minus_vec = (cam_est_vec - cam_obs_vec);
    data_derv = 2 * ((minus_vec)' * cam_est_derv)';

    % Regular Term
    regular_vec = ParaSet.gradOptC * d_depth_coarse_vec;
    regular_derv = ParaSet.gradOptC * regular_vec;

    derv_vec = data_derv + theta * regular_derv;
    norm_derv_vec = derv_vec / norm(derv_vec);
end