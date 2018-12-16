#!/bin/bash
#    --iovs '{"start_iov": "1355878225674241", "end_iov": "1395228716040193"}' \

getPayloadData.py \
    --plugin pluginSiPixelQuality_PayloadInspector \
    --plot plot_SiPixelQualityBadRocsSummary \
    --tag SiPixelQuality_byPCL_stuckTBM_v1 \
    --time_type Lumi \
    --iovs '{"start_iov": "1405858760098058", "end_iov": "1405858760098058"}' \
    --db Prod \
    --test;

