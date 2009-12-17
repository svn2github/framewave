//
// Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
// This software is subject to the Apache v2.0 License.
//
#ifndef __MAP_SSE2__
#define __MAP_SSE2__

//#include "SSEPlus_UNMAP_ALL.h"

//SSE
#define ssp_add_ps               ssp_add_ps_SSE
#define ssp_add_ss               ssp_add_ss_SSE
#define ssp_and_ps               ssp_and_ps_SSE
#define ssp_andnot_ps            ssp_andnot_ps_SSE
#define ssp_cmpeq_ps             ssp_cmpeq_ps_SSE
#define ssp_cmpeq_ss             ssp_cmpeq_ss_SSE
#define ssp_cmpge_ps             ssp_cmpge_ps_SSE
#define ssp_cmpge_ss             ssp_cmpge_ss_SSE
#define ssp_cmpgt_ps             ssp_cmpgt_ps_SSE
#define ssp_cmpgt_ss             ssp_cmpgt_ss_SSE
#define ssp_cmple_ps             ssp_cmple_ps_SSE
#define ssp_cmple_ss             ssp_cmple_ss_SSE
#define ssp_cmplt_ps             ssp_cmplt_ps_SSE
#define ssp_cmplt_ss             ssp_cmplt_ss_SSE
#define ssp_cmpneq_ps            ssp_cmpneq_ps_SSE
#define ssp_cmpneq_ss            ssp_cmpneq_ss_SSE
#define ssp_cmpnge_ps            ssp_cmpnge_ps_SSE
#define ssp_cmpnge_ss            ssp_cmpnge_ss_SSE
#define ssp_cmpngt_ps            ssp_cmpngt_ps_SSE
#define ssp_cmpngt_ss            ssp_cmpngt_ss_SSE
#define ssp_cmpnle_ps            ssp_cmpnle_ps_SSE
#define ssp_cmpnle_ss            ssp_cmpnle_ss_SSE
#define ssp_cmpnlt_ps            ssp_cmpnlt_ps_SSE
#define ssp_cmpnlt_ss            ssp_cmpnlt_ss_SSE
#define ssp_cmpord_ps            ssp_cmpord_ps_SSE
#define ssp_cmpord_ss            ssp_cmpord_ss_SSE
#define ssp_cmpunord_ps          ssp_cmpunord_ps_SSE
#define ssp_cmpunord_ss          ssp_cmpunord_ss_SSE
#define ssp_comieq_ss            ssp_comieq_ss_SSE
#define ssp_comige_ss            ssp_comige_ss_SSE
#define ssp_comigt_ss            ssp_comigt_ss_SSE
#define ssp_comile_ss            ssp_comile_ss_SSE
#define ssp_comilt_ss            ssp_comilt_ss_SSE
#define ssp_comineq_ss           ssp_comineq_ss_SSE
#define ssp_cvt_pi2ps            ssp_cvt_pi2ps_SSE
#define ssp_cvt_ps2pi            ssp_cvt_ps2pi_SSE
#define ssp_cvt_si2ss            ssp_cvt_si2ss_SSE
#define ssp_cvt_ss2si            ssp_cvt_ss2si_SSE
#define ssp_cvtsi64_ss           ssp_cvtsi64_ss_SSE
#define ssp_cvtss_f32            ssp_cvtss_f32_SSE
#define ssp_cvtss_si64           ssp_cvtss_si64_SSE
#define ssp_cvtt_ps2pi           ssp_cvtt_ps2pi_SSE
#define ssp_cvtt_ss2si           ssp_cvtt_ss2si_SSE
#define ssp_cvttss_si64          ssp_cvttss_si64_SSE
#define ssp_div_ps               ssp_div_ps_SSE
#define ssp_div_ss               ssp_div_ss_SSE
#define ssp_getcsr               ssp_getcsr_SSE
#define ssp_load_ps              ssp_load_ps_SSE
#define ssp_load_ps1             ssp_load_ps1_SSE
#define ssp_load_ss              ssp_load_ss_SSE
#define ssp_loadh_pi             ssp_loadh_pi_SSE
#define ssp_loadl_pi             ssp_loadl_pi_SSE
#define ssp_loadr_ps             ssp_loadr_ps_SSE
#define ssp_loadu_ps             ssp_loadu_ps_SSE
#define ssp_max_ps               ssp_max_ps_SSE
#define ssp_max_ss               ssp_max_ss_SSE
#define ssp_min_ps               ssp_min_ps_SSE
#define ssp_min_ss               ssp_min_ss_SSE
#define ssp_move_ss              ssp_move_ss_SSE
#define ssp_movehl_ps            ssp_movehl_ps_SSE
#define ssp_movelh_ps            ssp_movelh_ps_SSE
#define ssp_movemask_ps          ssp_movemask_ps_SSE
#define ssp_mul_ps               ssp_mul_ps_SSE
#define ssp_mul_ss               ssp_mul_ss_SSE
#define ssp_or_ps                ssp_or_ps_SSE
#define ssp_prefetch             ssp_prefetch_SSE
#define ssp_rcp_ps               ssp_rcp_ps_SSE
#define ssp_rcp_ss               ssp_rcp_ss_SSE
#define ssp_rsqrt_ps             ssp_rsqrt_ps_SSE
#define ssp_rsqrt_ss             ssp_rsqrt_ss_SSE
#define ssp_set_ps               ssp_set_ps_SSE
#define ssp_set_ps1              ssp_set_ps1_SSE
#define ssp_set_ss               ssp_set_ss_SSE
#define ssp_setcsr               ssp_setcsr_SSE
#define ssp_setr_ps              ssp_setr_ps_SSE
#define ssp_setzero_ps           ssp_setzero_ps_SSE
#define ssp_sfence               ssp_sfence_SSE
#define ssp_shuffle_ps           ssp_shuffle_ps_SSE
#define ssp_sqrt_ps              ssp_sqrt_ps_SSE
#define ssp_sqrt_ss              ssp_sqrt_ss_SSE
#define ssp_store_ps             ssp_store_ps_SSE
#define ssp_store_ps1            ssp_store_ps1_SSE
#define ssp_store_ss             ssp_store_ss_SSE
#define ssp_storeh_pi            ssp_storeh_pi_SSE
#define ssp_storel_pi            ssp_storel_pi_SSE
#define ssp_storer_ps            ssp_storer_ps_SSE
#define ssp_storeu_ps            ssp_storeu_ps_SSE
#define ssp_stream_ps            ssp_stream_ps_SSE
#define ssp_sub_ps               ssp_sub_ps_SSE
#define ssp_sub_ss               ssp_sub_ss_SSE
#define ssp_ucomieq_ss           ssp_ucomieq_ss_SSE
#define ssp_ucomige_ss           ssp_ucomige_ss_SSE
#define ssp_ucomigt_ss           ssp_ucomigt_ss_SSE
#define ssp_ucomile_ss           ssp_ucomile_ss_SSE
#define ssp_ucomilt_ss           ssp_ucomilt_ss_SSE
#define ssp_ucomineq_ss          ssp_ucomineq_ss_SSE
#define ssp_unpackhi_ps          ssp_unpackhi_ps_SSE
#define ssp_unpacklo_ps          ssp_unpacklo_ps_SSE
#define ssp_xor_ps               ssp_xor_ps_SSE

// SSE2 
#define ssp_add_epi16            ssp_add_epi16_SSE2       
#define ssp_add_epi32            ssp_add_epi32_SSE2       
#define ssp_add_epi64            ssp_add_epi64_SSE2
#define ssp_add_epi8             ssp_add_epi8_SSE2
#define ssp_add_pd               ssp_add_pd_SSE2
#define ssp_add_sd               ssp_add_sd_SSE2
#define ssp_add_si64             ssp_add_si64_SSE2
#define ssp_adds_epi16           ssp_adds_epi16_SSE2
#define ssp_adds_epi8            ssp_adds_epi8_SSE2
#define ssp_adds_epu16           ssp_adds_epu16_SSE2
#define ssp_adds_epu8            ssp_adds_epu8_SSE2
#define ssp_and_pd               ssp_and_pd_SSE2
#define ssp_and_si128            ssp_and_si128_SSE2
#define ssp_andnot_pd            ssp_andnot_pd_SSE2
#define ssp_andnot_si128         ssp_andnot_si128_SSE2
#define ssp_avg_epu16            ssp_avg_epu16_SSE2
#define ssp_avg_epu8             ssp_avg_epu8_SSE2
#define ssp_castpd_ps            ssp_castpd_ps_SSE2
#define ssp_castpd_si128         ssp_castpd_si128_SSE2
#define ssp_castps_pd            ssp_castps_pd_SSE2
#define ssp_castps_si128         ssp_castps_si128_SSE2
#define ssp_castsi128_pd         ssp_castsi128_pd_SSE2
#define ssp_castsi128_ps         ssp_castsi128_ps_SSE2
#define ssp_clflush              ssp_clflush_SSE2
#define ssp_cmpeq_epi16          ssp_cmpeq_epi16_SSE2
#define ssp_cmpeq_epi32          ssp_cmpeq_epi32_SSE2
#define ssp_cmpeq_epi8           ssp_cmpeq_epi8_SSE2
#define ssp_cmpeq_pd             ssp_cmpeq_pd_SSE2
#define ssp_cmpeq_sd             ssp_cmpeq_sd_SSE2
#define ssp_cmpge_pd             ssp_cmpge_pd_SSE2
#define ssp_cmpge_sd             ssp_cmpge_sd_SSE2
#define ssp_cmpgt_epi16          ssp_cmpgt_epi16_SSE2
#define ssp_cmpgt_epi32          ssp_cmpgt_epi32_SSE2
#define ssp_cmpgt_epi8           ssp_cmpgt_epi8_SSE2
#define ssp_cmpgt_pd             ssp_cmpgt_pd_SSE2
#define ssp_cmpgt_sd             ssp_cmpgt_sd_SSE2
#define ssp_cmple_pd             ssp_cmple_pd_SSE2
#define ssp_cmple_sd             ssp_cmple_sd_SSE2
#define ssp_cmplt_epi16          ssp_cmplt_epi16_SSE2
#define ssp_cmplt_epi32          ssp_cmplt_epi32_SSE2
#define ssp_cmplt_epi8           ssp_cmplt_epi8_SSE2
#define ssp_cmplt_pd             ssp_cmplt_pd_SSE2
#define ssp_cmplt_sd             ssp_cmplt_sd_SSE2
#define ssp_cmpneq_pd            ssp_cmpneq_pd_SSE2
#define ssp_cmpneq_sd            ssp_cmpneq_sd_SSE2
#define ssp_cmpnge_pd            ssp_cmpnge_pd_SSE2
#define ssp_cmpnge_sd            ssp_cmpnge_sd_SSE2
#define ssp_cmpngt_pd            ssp_cmpngt_pd_SSE2
#define ssp_cmpngt_sd            ssp_cmpngt_sd_SSE2
#define ssp_cmpnle_pd            ssp_cmpnle_pd_SSE2
#define ssp_cmpnle_sd            ssp_cmpnle_sd_SSE2
#define ssp_cmpnlt_pd            ssp_cmpnlt_pd_SSE2
#define ssp_cmpnlt_sd            ssp_cmpnlt_sd_SSE2
#define ssp_cmpord_pd            ssp_cmpord_pd_SSE2
#define ssp_cmpord_sd            ssp_cmpord_sd_SSE2
#define ssp_cmpunord_pd          ssp_cmpunord_pd_SSE2
#define ssp_cmpunord_sd          ssp_cmpunord_sd_SSE2
#define ssp_comieq_sd            ssp_comieq_sd_SSE2
#define ssp_comige_sd            ssp_comige_sd_SSE2
#define ssp_comigt_sd            ssp_comigt_sd_SSE2
#define ssp_comile_sd            ssp_comile_sd_SSE2
#define ssp_comilt_sd            ssp_comilt_sd_SSE2
#define ssp_comineq_sd           ssp_comineq_sd_SSE2
#define ssp_cvtepi32_pd          ssp_cvtepi32_pd_SSE2
#define ssp_cvtepi32_ps          ssp_cvtepi32_ps_SSE2
#define ssp_cvtpd_epi32          ssp_cvtpd_epi32_SSE2
#define ssp_cvtpd_pi32           ssp_cvtpd_pi32_SSE2
#define ssp_cvtpd_ps             ssp_cvtpd_ps_SSE2
#define ssp_cvtpi32_pd           ssp_cvtpi32_pd_SSE2
#define ssp_cvtps_epi32          ssp_cvtps_epi32_SSE2
#define ssp_cvtps_pd             ssp_cvtps_pd_SSE2
#define ssp_cvtsd_f64            ssp_cvtsd_f64_SSE2
#define ssp_cvtsd_si32           ssp_cvtsd_si32_SSE2
#define ssp_cvtsd_si64           ssp_cvtsd_si64_SSE2
#define ssp_cvtsd_ss             ssp_cvtsd_ss_SSE2
#define ssp_cvtsi128_si32        ssp_cvtsi128_si32_SSE2
#define ssp_cvtsi128_si64        ssp_cvtsi128_si64_SSE2
#define ssp_cvtsi32_sd           ssp_cvtsi32_sd_SSE2
#define ssp_cvtsi32_si128        ssp_cvtsi32_si128_SSE2
#define ssp_cvtsi64_sd           ssp_cvtsi64_sd_SSE2
#define ssp_cvtsi64_si128        ssp_cvtsi64_si128_SSE2
#define ssp_cvtss_sd             ssp_cvtss_sd_SSE2
#define ssp_cvttpd_epi32         ssp_cvttpd_epi32_SSE2
#define ssp_cvttpd_pi32          ssp_cvttpd_pi32_SSE2
#define ssp_cvttps_epi32         ssp_cvttps_epi32_SSE2
#define ssp_cvttsd_si32          ssp_cvttsd_si32_SSE2
#define ssp_cvttsd_si64          ssp_cvttsd_si64_SSE2
#define ssp_div_pd               ssp_div_pd_SSE2
#define ssp_div_sd               ssp_div_sd_SSE2
#define ssp_extract_epi16        ssp_extract_epi16_SSE2
#define ssp_insert_epi16         ssp_insert_epi16_SSE2
#define ssp_lfence               ssp_lfence_SSE2
#define ssp_load_pd              ssp_load_pd_SSE2
#define ssp_load_sd              ssp_load_sd_SSE2
#define ssp_load_si128           ssp_load_si128_SSE2
#define ssp_load1_pd             ssp_load1_pd_SSE2
#define ssp_loadh_pd             ssp_loadh_pd_SSE2
#define ssp_loadl_epi64          ssp_loadl_epi64_SSE2
#define ssp_loadl_pd             ssp_loadl_pd_SSE2
#define ssp_loadr_pd             ssp_loadr_pd_SSE2
#define ssp_loadu_pd             ssp_loadu_pd_SSE2
#define ssp_loadu_si128          ssp_loadu_si128_SSE2
#define ssp_madd_epi16           ssp_madd_epi16_SSE2
#define ssp_maskmoveu_si128      ssp_maskmoveu_si128_SSE2
#define ssp_max_epi16            ssp_max_epi16_SSE2
#define ssp_max_epu8             ssp_max_epu8_SSE2
#define ssp_max_pd               ssp_max_pd_SSE2
#define ssp_max_sd               ssp_max_sd_SSE2
#define ssp_mfence               ssp_mfence_SSE2
#define ssp_min_epi16            ssp_min_epi16_SSE2
#define ssp_min_epu8             ssp_min_epu8_SSE2
#define ssp_min_pd               ssp_min_pd_SSE2
#define ssp_min_sd               ssp_min_sd_SSE2
#define ssp_move_epi64           ssp_move_epi64_SSE2
#define ssp_move_sd              ssp_move_sd_SSE2
#define ssp_movemask_epi8        ssp_movemask_epi8_SSE2
#define ssp_movemask_pd          ssp_movemask_pd_SSE2
#define ssp_movepi64_pi64        ssp_movepi64_pi64_SSE2
#define ssp_movpi64_epi64        ssp_movpi64_epi64_SSE2
#define ssp_mul_epu32            ssp_mul_epu32_SSE2
#define ssp_mul_pd               ssp_mul_pd_SSE2
#define ssp_mul_sd               ssp_mul_sd_SSE2
#define ssp_mul_su32             ssp_mul_su32_SSE2
#define ssp_mulhi_epi16          ssp_mulhi_epi16_SSE2
#define ssp_mulhi_epu16          ssp_mulhi_epu16_SSE2
#define ssp_mullo_epi16          ssp_mullo_epi16_SSE2
#define ssp_or_pd                ssp_or_pd_SSE2
#define ssp_or_si128             ssp_or_si128_SSE2
#define ssp_packs_epi16          ssp_packs_epi16_SSE2
#define ssp_packs_epi32          ssp_packs_epi32_SSE2
#define ssp_packus_epi16         ssp_packus_epi16_SSE2
#define ssp_pause                ssp_pause_SSE2
#define ssp_sad_epu8             ssp_sad_epu8_SSE2
#define ssp_set_epi16            ssp_set_epi16_SSE2
#define ssp_set_epi32            ssp_set_epi32_SSE2
#define ssp_set_epi64            ssp_set_epi64_SSE2
#define ssp_set_epi8             ssp_set_epi8_SSE2
#define ssp_set_pd               ssp_set_pd_SSE2
#define ssp_set_sd               ssp_set_sd_SSE2
#define ssp_set1_epi16           ssp_set1_epi16_SSE2
#define ssp_set1_epi32           ssp_set1_epi32_SSE2
#define ssp_set1_epi64           ssp_set1_epi64_SSE2
#define ssp_set1_epi8            ssp_set1_epi8_SSE2
#define ssp_set1_pd              ssp_set1_pd_SSE2
#define ssp_setl_epi64           ssp_setl_epi64_SSE2
#define ssp_setr_epi16           ssp_setr_epi16_SSE2
#define ssp_setr_epi32           ssp_setr_epi32_SSE2
#define ssp_setr_epi64           ssp_setr_epi64_SSE2
#define ssp_setr_epi8            ssp_setr_epi8_SSE2
#define ssp_setr_pd              ssp_setr_pd_SSE2
#define ssp_setzero_pd           ssp_setzero_pd_SSE2
#define ssp_setzero_si128        ssp_setzero_si128_SSE2
#define ssp_shuffle_epi32        ssp_shuffle_epi32_SSE2
#define ssp_shuffle_pd           ssp_shuffle_pd_SSE2
#define ssp_shufflehi_epi16      ssp_shufflehi_epi16_SSE2
#define ssp_shufflelo_epi16      ssp_shufflelo_epi16_SSE2
#define ssp_sll_epi16            ssp_sll_epi16_SSE2
#define ssp_sll_epi32            ssp_sll_epi32_SSE2
#define ssp_sll_epi64            ssp_sll_epi64_SSE2
#define ssp_slli_epi16           ssp_slli_epi16_SSE2
#define ssp_slli_epi32           ssp_slli_epi32_SSE2
#define ssp_slli_epi64           ssp_slli_epi64_SSE2
#define ssp_slli_si128           ssp_slli_si128_SSE2
#define ssp_sqrt_pd              ssp_sqrt_pd_SSE2
#define ssp_sqrt_sd              ssp_sqrt_sd_SSE2
#define ssp_sra_epi16            ssp_sra_epi16_SSE2
#define ssp_sra_epi32            ssp_sra_epi32_SSE2
#define ssp_srai_epi16           ssp_srai_epi16_SSE2
#define ssp_srai_epi32           ssp_srai_epi32_SSE2
#define ssp_srl_epi16            ssp_srl_epi16_SSE2
#define ssp_srl_epi32            ssp_srl_epi32_SSE2
#define ssp_srl_epi64            ssp_srl_epi64_SSE2
#define ssp_srli_epi16           ssp_srli_epi16_SSE2
#define ssp_srli_epi32           ssp_srli_epi32_SSE2
#define ssp_srli_epi64           ssp_srli_epi64_SSE2
#define ssp_srli_si128           ssp_srli_si128_SSE2
#define ssp_store_pd             ssp_store_pd_SSE2
#define ssp_store_sd             ssp_store_sd_SSE2
#define ssp_store_si128          ssp_store_si128_SSE2
#define ssp_store1_pd            ssp_store1_pd_SSE2
#define ssp_storeh_pd            ssp_storeh_pd_SSE2
#define ssp_storel_epi64         ssp_storel_epi64_SSE2
#define ssp_storel_pd            ssp_storel_pd_SSE2
#define ssp_storer_pd            ssp_storer_pd_SSE2
#define ssp_storeu_pd            ssp_storeu_pd_SSE2
#define ssp_storeu_si128         ssp_storeu_si128_SSE2
#define ssp_stream_pd            ssp_stream_pd_SSE2
#define ssp_stream_si128         ssp_stream_si128_SSE2
#define ssp_stream_si32          ssp_stream_si32_SSE2
#define ssp_sub_epi16            ssp_sub_epi16_SSE2
#define ssp_sub_epi32            ssp_sub_epi32_SSE2
#define ssp_sub_epi64            ssp_sub_epi64_SSE2
#define ssp_sub_epi8             ssp_sub_epi8_SSE2
#define ssp_sub_pd               ssp_sub_pd_SSE2
#define ssp_sub_sd               ssp_sub_sd_SSE2
#define ssp_sub_si64             ssp_sub_si64_SSE2
#define ssp_subs_epi16           ssp_subs_epi16_SSE2
#define ssp_subs_epi8            ssp_subs_epi8_SSE2
#define ssp_subs_epu16           ssp_subs_epu16_SSE2
#define ssp_subs_epu8            ssp_subs_epu8_SSE2
#define ssp_ucomieq_sd           ssp_ucomieq_sd_SSE2
#define ssp_ucomige_sd           ssp_ucomige_sd_SSE2
#define ssp_ucomigt_sd           ssp_ucomigt_sd_SSE2
#define ssp_ucomile_sd           ssp_ucomile_sd_SSE2
#define ssp_ucomilt_sd           ssp_ucomilt_sd_SSE2
#define ssp_ucomineq_sd          ssp_ucomineq_sd_SSE2
#define ssp_unpackhi_epi16       ssp_unpackhi_epi16_SSE2
#define ssp_unpackhi_epi32       ssp_unpackhi_epi32_SSE2
#define ssp_unpackhi_epi64       ssp_unpackhi_epi64_SSE2
#define ssp_unpackhi_epi8        ssp_unpackhi_epi8_SSE2
#define ssp_unpackhi_pd          ssp_unpackhi_pd_SSE2
#define ssp_unpacklo_epi16       ssp_unpacklo_epi16_SSE2
#define ssp_unpacklo_epi32       ssp_unpacklo_epi32_SSE2
#define ssp_unpacklo_epi64       ssp_unpacklo_epi64_SSE2
#define ssp_unpacklo_epi8        ssp_unpacklo_epi8_SSE2
#define ssp_unpacklo_pd          ssp_unpacklo_pd_SSE2
#define ssp_xor_pd               ssp_xor_pd_SSE2
#define ssp_xor_si128            ssp_xor_si128_SSE2
#define ssp_cvttss_si32          ssp_cvttss_si32_SSE2
#define ssp_cvttps_pi32          ssp_cvttps_pi32_SSE2
#define ssp_maskmove_si64        ssp_maskmove_si64_SSE2
#define ssp_avg_pu8              ssp_avg_pu8_SSE2
#define ssp_set1_ps              ssp_set1_ps_SSE2
#define ssp_load1_ps             ssp_load1_ps_SSE2
#define ssp_store1_ps            ssp_store1_ps_SSE2

// SSE3
#define ssp_addsub_pd            ssp_addsub_pd_SSE2        
#define ssp_addsub_ps            ssp_addsub_ps_SSE2        
#define ssp_hadd_pd              ssp_hadd_pd_SSE2          
#define ssp_hadd_ps              ssp_hadd_ps_SSE2          
#define ssp_hsub_pd2             ssp_hsub_pd_SSE2          
#define ssp_hsub_ps              ssp_hsub_ps_SSE2          
#define ssp_lddqu_si128          ssp_lddqu_si128_SSE2      
#define ssp_loaddup_pd           ssp_loaddup_pd_SSE2       
#define ssp_movedup_pd           ssp_movedup_pd_SSE2       
#define ssp_movehdup_ps          ssp_movehdup_ps_SSE2      
#define ssp_moveldup_ps          ssp_moveldup_ps_SSE2    

// SSSE3
#define ssp_abs_epi16            ssp_abs_epi16_SSE2        
#define ssp_abs_epi32            ssp_abs_epi32_SSE2        
#define ssp_abs_epi8             ssp_abs_epi8_SSE2         
#define ssp_abs_pi16             ssp_abs_pi16_SSE2         
#define ssp_abs_pi32             ssp_abs_pi32_SSE2         
#define ssp_abs_pi8              ssp_abs_pi8_SSE2          
#define ssp_alignr_epi8          ssp_alignr_epi8_SSE2      
#define ssp_alignr_pi8           ssp_alignr_pi8_SSE2       
#define ssp_hadd_epi16           ssp_hadd_epi16_SSE2       
#define ssp_hadd_epi32           ssp_hadd_epi32_SSE2       
#define ssp_hadd_pi16            ssp_hadd_pi16_SSE2        
#define ssp_hadd_pi32            ssp_hadd_pi32_SSE2        
#define ssp_hadds_epi16          ssp_hadds_epi16_SSE2      
#define ssp_hadds_pi16           ssp_hadds_pi16_SSE2       
#define ssp_hsub_epi16           ssp_hsub_epi16_SSE2       
#define ssp_hsub_epi32           ssp_hsub_epi32_SSE2       
#define ssp_hsub_pi16            ssp_hsub_pi16_SSE2        
#define ssp_hsub_pi32            ssp_hsub_pi32_SSE2        
#define ssp_hsubs_epi16          ssp_hsubs_epi16_SSE2      
#define ssp_hsubs_pi16           ssp_hsubs_pi16_SSE2       
#define ssp_maddubs_epi16        ssp_maddubs_epi16_SSE2    
#define ssp_maddubs_pi16         ssp_maddubs_pi16_SSE2     
#define ssp_mulhrs_epi16         ssp_mulhrs_epi16_SSE2     
#define ssp_mulhrs_pi16          ssp_mulhrs_pi16_SSE2      
#define ssp_shuffle_epi8         ssp_shuffle_epi8_SSE2     
#define ssp_shuffle_pi8          ssp_shuffle_pi8_SSE2      
#define ssp_sign_epi16           ssp_sign_epi16_SSE2       
#define ssp_sign_epi32           ssp_sign_epi32_SSE2       
#define ssp_sign_epi8            ssp_sign_epi8_SSE2        
#define ssp_sign_pi16            ssp_sign_pi16_SSE2        
#define ssp_sign_pi32            ssp_sign_pi32_SSE2        
#define ssp_sign_pi8             ssp_sign_pi8_SSE2     

//SSE4A
#define ssp_extract_si64_SSE2    ssp_extract_si64_SSE2     
#define ssp_extracti_si64_SSE2   ssp_extracti_si64_SSE2    
#define ssp_insert_si64_SSE2     ssp_insert_si64_SSE2      
#define ssp_inserti_si64_SSE2    ssp_inserti_si64_SSE2     
#define ssp_stream_sd_SSE2       ssp_stream_sd_SSE2        
#define ssp_stream_ss_SSE2       ssp_stream_ss_SSE2

// SSE4.1
#define ssp_blend_epi16          ssp_blend_epi16_SSE2      
#define ssp_blend_pd             ssp_blend_pd_SSE2         
#define ssp_blend_ps             ssp_blend_ps_SSE2         
#define ssp_blendv_epi8          ssp_blendv_epi8_SSE2      
#define ssp_blendv_pd            ssp_blendv_pd_SSE2        
#define ssp_blendv_ps            ssp_blendv_ps_SSE2        
#define ssp_ceil_pd              ssp_ceil_pd_SSE2          
#define ssp_ceil_ps              ssp_ceil_ps_SSE2          
#define ssp_ceil_sd              ssp_ceil_sd_SSE2          
#define ssp_ceil_ss              ssp_ceil_ss_SSE2          
#define ssp_cmpeq_epi64          ssp_cmpeq_epi64_SSE2      
#define ssp_cvtepi16_epi32       ssp_cvtepi16_epi32_SSE2   
#define ssp_cvtepi16_epi64       ssp_cvtepi16_epi64_SSE2   
#define ssp_cvtepi32_epi64       ssp_cvtepi32_epi64_SSE2   
#define ssp_cvtepi8_epi16        ssp_cvtepi8_epi16_SSE2    
#define ssp_cvtepi8_epi32        ssp_cvtepi8_epi32_SSE2    
#define ssp_cvtepi8_epi64        ssp_cvtepi8_epi64_SSE2    
#define ssp_cvtepu16_epi32       ssp_cvtepu16_epi32_SSE2   
#define ssp_cvtepu16_epi64       ssp_cvtepu16_epi64_SSE2   
#define ssp_cvtepu32_epi64       ssp_cvtepu32_epi64_SSE2   
#define ssp_cvtepu8_epi16        ssp_cvtepu8_epi16_SSE2    
#define ssp_cvtepu8_epi32        ssp_cvtepu8_epi32_SSE2    
#define ssp_cvtepu8_epi64        ssp_cvtepu8_epi64_SSE2    
#define ssp_dp_pd                ssp_dp_pd_SSE2            
#define ssp_dp_ps                ssp_dp_ps_SSE2            
#define ssp_extract_epi32        ssp_extract_epi32_SSE2    
#define ssp_extract_epi64        ssp_extract_epi64_SSE2    
#define ssp_extract_epi8         ssp_extract_epi8_SSE2     
#define ssp_extract_ps           ssp_extract_ps_SSE2       
#define ssp_floor_pd             ssp_floor_pd_SSE2         
#define ssp_floor_ps             ssp_floor_ps_SSE2         
#define ssp_floor_sd             ssp_floor_sd_SSE2         
#define ssp_floor_ss             ssp_floor_ss_SSE2         
#define ssp_insert_epi32         ssp_insert_epi32_SSE2     
#define ssp_insert_epi64         ssp_insert_epi64_SSE2     
#define ssp_insert_epi8          ssp_insert_epi8_SSE2      
#define ssp_insert_ps            ssp_insert_ps_SSE2        
#define ssp_max_epi32            ssp_max_epi32_SSE2        
#define ssp_max_epi8             ssp_max_epi8_SSE2         
#define ssp_max_epu16            ssp_max_epu16_SSE2        
#define ssp_max_epu32            ssp_max_epu32_SSE2        
#define ssp_min_epi32            ssp_min_epi32_SSE2        
#define ssp_min_epi8             ssp_min_epi8_SSE2         
#define ssp_min_epu16            ssp_min_epu16_SSE2        
#define ssp_min_epu32            ssp_min_epu32_SSE2        
#define ssp_minpos_epu16         ssp_minpos_epu16_SSE2     
#define ssp_mpsadbw_epu8         ssp_mpsadbw_epu8_SSE2     
#define ssp_mul_epi32            ssp_mul_epi32_SSE2        
#define ssp_mullo_epi32          ssp_mullo_epi32_SSE2      
#define ssp_packus_epi32         ssp_packus_epi32_SSE2     
#define ssp_round_pd             ssp_round_pd_SSE2         
#define ssp_round_ps             ssp_round_ps_SSE2         
#define ssp_round_sd             ssp_round_sd_SSE2         
#define ssp_round_ss             ssp_round_ss_SSE2         
#define ssp_stream_load_si128     ssp_stream_load_si128_SSE2
#define ssp_testc_si128          ssp_testc_si128_SSE2      
#define ssp_testnzc_si128        ssp_testnzc_si128_SSE2    
#define ssp_testz_si128          ssp_testz_si128_SSE2      
                                      
//SSE4.2                              
#define ssp_cmpestra             ssp_cmpestra_SSE2         
#define ssp_cmpestrc             ssp_cmpestrc_SSE2         
#define ssp_cmpestri             ssp_cmpestri_SSE2         
#define ssp_cmpestrm             ssp_cmpestrm_SSE2         
#define ssp_cmpestro             ssp_cmpestro_SSE2         
#define ssp_cmpestrs             ssp_cmpestrs_SSE2         
#define ssp_cmpestrz             ssp_cmpestrz_SSE2         
#define ssp_cmpgt_epi64          ssp_cmpgt_epi64_SSE2      
#define ssp_cmpistra             ssp_cmpistra_SSE2         
#define ssp_cmpistrc             ssp_cmpistrc_SSE2         
#define ssp_cmpistri             ssp_cmpistri_SSE2         
#define ssp_cmpistrm             ssp_cmpistrm_SSE2         
#define ssp_cmpistro             ssp_cmpistro_SSE2         
#define ssp_cmpistrs             ssp_cmpistrs_SSE2         
#define ssp_cmpistrz             ssp_cmpistrz_SSE2         
#define ssp_crc32_u16            ssp_crc32_u16_SSE2 
#define ssp_crc32_u32            ssp_crc32_u32_SSE2 
#define ssp_crc32_u64            ssp_crc32_u64_SSE2 
#define ssp_crc32_u8             ssp_crc32_u8_SSE2  
#define ssp_popcnt_u32           ssp_popcnt_u32_SSE2       
#define ssp_popcnt_u64           ssp_popcnt_u64_SSE2  

// Arithmetic
#define ssp_arithmetic_hadd4_epi16  ssp_arithmetic_hadd4_epi16_SSE2
#define ssp_arithmetic_hadd4_dup_ps ssp_arithmetic_hadd4_dup_ps_SSE2

// Logical
#define ssp_logical_bitwise_select  ssp_logical_bitwise_select_SSE2

// Convert
#define ssp_convert_3c_3p_epi8 ssp_convert_3c_3p_epi8_SSE2
#define ssp_convert_3p_3c_epi8 ssp_convert_3p_3c_epi8_SSE2

#define ssp_convert_3c_3p_epi16 ssp_convert_3c_3p_epi16_SSE2
#define ssp_convert_3p_3c_epi16 ssp_convert_3p_3c_epi16_SSE2

#define ssp_convert_3c_3p_epi32 ssp_convert_3c_3p_epi32_SSE2
#define ssp_convert_3p_3c_epi32 ssp_convert_3p_3c_epi32_SSE2

#define ssp_convert_4c_4p_epi8 ssp_convert_4c_4p_epi8_SSE2
#define ssp_convert_4p_4c_epi8 ssp_convert_4p_4c_epi8_SSE2

#define ssp_convert_4c_4p_epi16 ssp_convert_4c_4p_epi16_SSE2
#define ssp_convert_4p_4c_epi16 ssp_convert_4p_4c_epi16_SSE2

#define ssp_convert_4c_4p_epi32 ssp_convert_4c_4p_epi32_SSE2
#define ssp_convert_4p_4c_epi32 ssp_convert_4p_4c_epi32_SSE2

#endif //__MAP_SSE2__
