function beliefField = BeliefFieldIteration(beliefFieldLast, ...
    pattern, ...
    last_x_gt, ...
    last_y_gt, ...
    last_depth_mat, ...
    lineA, ...
    lineB, ...
    lineC, ...
    viewportMatrix, ...
    voxelSize, ...
    halfVoxelRange)

    beliefField = zeros(size(beliefFieldLast));
    halfNeighborRange = 7;

    % Iteration: for every voxel in the beliefField
    for h = viewportMatrix(2, 1):viewportMatrix(2, 2)
        for w = viewportMatrix(1, 1):viewportMatrix(1, 2)

            x_p = last_x_gt(h, w);
            y_p = last_y_gt(h, w);
            c_xy = GetColorByXYpro(x_p, y_p, pattern);

            for d_idx = 1:halfVoxelRange * 2 + 1

                % Calculate depth value
                delta_depth = (d_idx - halfVoxelRange + 1) * voxelSize;
                depth = last_depth_mat(h, w) + delta_depth;
                % Get xpro, ypro from depth
                xpro = depth2xpro(w, h, depth);
                ypro = xpro2ypro(w, h, xpro, lineA, lineB, lineC);
                p_xy = GetColorByXYpro(xpro, ypro, pattern);
                % Set initial value
                tmp_exp = Phi_p(delta_depth, c_xy, p_xy);
                % calculate sum
                sum_exp = 0;
                for hd = -halfNeighborRange:halfNeighborRange
                    for wd = -halfNeighborRange:halfNeighborRange
                        h_neighbor = h + hd;
                        w_neighbor = w + wd;
                        if (h_neighbor > viewportMatrix(2, 1)) ...
                            && (h_neighbor < viewportMatrix(2, 2)) ...
                            && (w_neighbor > viewportMatrix(1, 1)) ...
                            && (w_neighbor < viewportMatrix(1, 2))
                            distance_value = K_distance(w, h, ...
                                w_neighbor, h_neighbor);
                            for d_idx_n = 1:2*halfVoxelRange+1
                                delta_depth_n = (d_idx_n - halfVoxelRange + 1) * voxelSize;
                                depth_n = last_depth_mat(h_neighbor, w_neighbor) + delta_depth_n;
                                mu_value = Mu_depth(depth, depth_n);
                                sum_exp = sum_exp + distance_value ...
                                    * mu_value ...
                                    * beliefFieldLast(h_neighbor, w_neighbor, d_idx_n);
                            end
                        end
                    end
                end
                alpha = color_alpha(c_xy, p_xy);
                beliefField(h, w, d_idx) = alpha * exp(-tmp_exp-sum_exp);
            end
            sum_value = sum(beliefField(h, w, :));
            beliefField(h, w, :) = beliefField(h, w, :) / sum_value;
        end
        if mod(h, 10) == 0
            fprintf('.')
        end
    end

end
