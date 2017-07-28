function delta_depth_mats = GetDeltaDepthsFromBeliefField(belief_field, ...
    mask_mats, ...
    viewportMatrix, ...
    cal_para)
    [LAYER_NUM, ~] = size(belief_field);
    
    delta_depth_mats = cell(LAYER_NUM, 1);
    for l = 1:LAYER_NUM
        for h = viewportMatrix(2, 1):viewportMatrix(2, 2)
            for w = viewportMatrix(1, 1):viewportMatrix(1, 2)
                [~, max_idx] = max(belief_field{h, w});
                delta_depth = (max_idx - halfVoxelRange - 1) * voxelSize;
                delta_depth_mats(h, w) = delta_depth;
            end
        end
    end
    
end
