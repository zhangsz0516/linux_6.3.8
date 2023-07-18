/* SPDX-License-Identifier: (GPL-2.0-only OR BSD-3-Clause) */
/*
 * This file is provided under a dual BSD/GPLv2 license.  When using or
 * redistributing this file, you may do so under either license.
 *
 * Copyright(c) 2022 Intel Corporation. All rights reserved.
 */

#ifndef __INCLUDE_SOUND_SOF_IPC4_TOPOLOGY_H__
#define __INCLUDE_SOUND_SOF_IPC4_TOPOLOGY_H__

#include <sound/sof/ipc4/header.h>

#define SOF_IPC4_FW_PAGE_SIZE BIT(12)
#define SOF_IPC4_FW_PAGE(x) ((((x) + BIT(12) - 1) & ~(BIT(12) - 1)) >> 12)
#define SOF_IPC4_FW_ROUNDUP(x) (((x) + BIT(6) - 1) & (~(BIT(6) - 1)))

#define SOF_IPC4_MODULE_LOAD_TYPE		GENMASK(3, 0)
#define SOF_IPC4_MODULE_AUTO_START		BIT(4)
/*
 * Two module schedule domains in fw :
 * LL domain - Low latency domain
 * DP domain - Data processing domain
 * The LL setting should be equal to !DP setting
 */
#define SOF_IPC4_MODULE_LL		BIT(5)
#define SOF_IPC4_MODULE_DP		BIT(6)
#define SOF_IPC4_MODULE_LIB_CODE		BIT(7)

#define SOF_IPC4_MODULE_INSTANCE_LIST_ITEM_SIZE 12
#define SOF_IPC4_PIPELINE_OBJECT_SIZE 448
#define SOF_IPC4_DATA_QUEUE_OBJECT_SIZE 128
#define SOF_IPC4_LL_TASK_OBJECT_SIZE 72
#define SOF_IPC4_DP_TASK_OBJECT_SIZE 104
#define SOF_IPC4_DP_TASK_LIST_SIZE (12 + 8)
#define SOF_IPC4_LL_TASK_LIST_ITEM_SIZE 12
#define SOF_IPC4_FW_MAX_PAGE_COUNT 20
#define SOF_IPC4_FW_MAX_QUEUE_COUNT 8

/* Node index and mask applicable for host copier and ALH/HDA type DAI copiers */
#define SOF_IPC4_NODE_INDEX_MASK	0xFF
#define SOF_IPC4_NODE_INDEX(x)	((x) & SOF_IPC4_NODE_INDEX_MASK)
#define SOF_IPC4_NODE_TYPE(x)  ((x) << 8)

/* Node ID for SSP type DAI copiers */
#define SOF_IPC4_NODE_INDEX_INTEL_SSP(x) (((x) & 0xf) << 4)

/* Node ID for DMIC type DAI copiers */
#define SOF_IPC4_NODE_INDEX_INTEL_DMIC(x) ((x) & 0x7)

#define SOF_IPC4_GAIN_ALL_CHANNELS_MASK 0xffffffff
#define SOF_IPC4_VOL_ZERO_DB	0x7fffffff

#define ALH_MAX_NUMBER_OF_GTW   16

#define SOF_IPC4_INVALID_NODE_ID	0xffffffff

/*
 * The base of multi-gateways. Multi-gateways addressing starts from
 * ALH_MULTI_GTW_BASE and there are ALH_MULTI_GTW_COUNT multi-sources
 * and ALH_MULTI_GTW_COUNT multi-sinks available.
 * Addressing is continuous from ALH_MULTI_GTW_BASE to
 * ALH_MULTI_GTW_BASE + ALH_MULTI_GTW_COUNT - 1.
 */
#define ALH_MULTI_GTW_BASE	0x50
/* A magic number from FW */
#define ALH_MULTI_GTW_COUNT	8

enum sof_ipc4_copier_module_config_params {
/*
 * Use LARGE_CONFIG_SET to initialize timestamp event. Ipc mailbox must
 * contain properly built CopierConfigTimestampInitData struct.
 */
	SOF_IPC4_COPIER_MODULE_CFG_PARAM_TIMESTAMP_INIT = 1,
/*
 * Use LARGE_CONFIG_SET to initialize copier sink. Ipc mailbox must contain
 * properly built CopierConfigSetSinkFormat struct.
 */
	SOF_IPC4_COPIER_MODULE_CFG_PARAM_SET_SINK_FORMAT,
/*
 * Use LARGE_CONFIG_SET to initialize and enable on Copier data segment
 * event. Ipc mailbox must contain properly built DataSegmentEnabled struct.
 */
	SOF_IPC4_COPIER_MODULE_CFG_PARAM_DATA_SEGMENT_ENABLED,
/*
 * Use LARGE_CONFIG_GET to retrieve Linear Link Position (LLP) value for non
 * HD-A gateways.
 */
	SOF_IPC4_COPIER_MODULE_CFG_PARAM_LLP_READING,
/*
 * Use LARGE_CONFIG_GET to retrieve Linear Link Position (LLP) value for non
 * HD-A gateways and corresponding total processed data
 */
	SOF_IPC4_COPIER_MODULE_CFG_PARAM_LLP_READING_EXTENDED,
/*
 * Use LARGE_CONFIG_SET to setup attenuation on output pins. Data is just uint32_t.
 * note Config is only allowed when output pin is set up for 32bit and source
 * is connected to Gateway
 */
	SOF_IPC4_COPIER_MODULE_CFG_ATTENUATION,
};

struct sof_ipc4_copier_config_set_sink_format {
/* Id of sink */
	u32 sink_id;
/*
 * Input format used by the source
 * attention must be the same as present if already initialized.
 */
	struct sof_ipc4_audio_format source_fmt;
/* Output format used by the sink */
	struct sof_ipc4_audio_format sink_fmt;
} __packed __aligned(4);

/**
 * struct sof_ipc4_pipeline - pipeline config data
 * @priority: Priority of this pipeline
 * @lp_mode: Low power mode
 * @mem_usage: Memory usage
 * @state: Pipeline state
 * @msg: message structure for pipeline
 * @skip_during_fe_trigger: skip triggering this pipeline during the FE DAI trigger
 */
struct sof_ipc4_pipeline {
	uint32_t priority;
	uint32_t lp_mode;
	uint32_t mem_usage;
	int state;
	struct sof_ipc4_msg msg;
	bool skip_during_fe_trigger;
};

/**
 * struct sof_ipc4_multi_pipeline_data - multi pipeline trigger IPC data
 * @count: Number of pipelines to be triggered
 * @pipeline_ids: Flexible array of IDs of the pipelines to be triggered
 */
struct ipc4_pipeline_set_state_data {
	u32 count;
	DECLARE_FLEX_ARRAY(u32, pipeline_ids);
} __packed;

/**
 * struct sof_ipc4_available_audio_format - Available audio formats
 * @base_config: Available base config
 * @out_audio_fmt: Available output audio format
 * @ref_audio_fmt: Reference audio format to match runtime audio format
 * @dma_buffer_size: Available Gateway DMA buffer size (in bytes)
 * @audio_fmt_num: Number of available audio formats
 */
struct sof_ipc4_available_audio_format {
	struct sof_ipc4_base_module_cfg *base_config;
	struct sof_ipc4_audio_format *out_audio_fmt;
	struct sof_ipc4_audio_format *ref_audio_fmt;
	u32 *dma_buffer_size;
	int audio_fmt_num;
};

/**
 * struct sof_copier_gateway_cfg - IPC gateway configuration
 * @node_id: ID of Gateway Node
 * @dma_buffer_size: Preferred Gateway DMA buffer size (in bytes)
 * @config_length: Length of gateway node configuration blob specified in #config_data
 * config_data: Gateway node configuration blob
 */
struct sof_copier_gateway_cfg {
	uint32_t node_id;
	uint32_t dma_buffer_size;
	uint32_t config_length;
	uint32_t config_data[];
};

/**
 * struct sof_ipc4_copier_data - IPC data for copier
 * @base_config: Base configuration including input audio format
 * @out_format: Output audio format
 * @copier_feature_mask: Copier feature mask
 * @gtw_cfg: Gateway configuration
 */
struct sof_ipc4_copier_data {
	struct sof_ipc4_base_module_cfg base_config;
	struct sof_ipc4_audio_format out_format;
	uint32_t copier_feature_mask;
	struct sof_copier_gateway_cfg gtw_cfg;
};

/**
 * struct sof_ipc4_gtw_attributes: Gateway attributes
 * @lp_buffer_alloc: Gateway data requested in low power memory
 * @alloc_from_reg_file: Gateway data requested in register file memory
 * @rsvd: reserved for future use
 */
struct sof_ipc4_gtw_attributes {
	uint32_t lp_buffer_alloc : 1;
	uint32_t alloc_from_reg_file : 1;
	uint32_t rsvd : 30;
};

/** struct sof_ipc4_alh_multi_gtw_cfg: ALH gateway cfg data
 * @count: Number of streams (valid items in mapping array)
 * @alh_id: ALH stream id of a single ALH stream aggregated
 * @channel_mask: Channel mask
 * @mapping: ALH streams
 */
struct sof_ipc4_alh_multi_gtw_cfg {
	uint32_t count;
	struct {
		uint32_t alh_id;
		uint32_t channel_mask;
	} mapping[ALH_MAX_NUMBER_OF_GTW];
} __packed;

/** struct sof_ipc4_alh_configuration_blob: ALH blob
 * @gw_attr: Gateway attributes
 * @alh_cfg: ALH configuration data
 */
struct sof_ipc4_alh_configuration_blob {
	struct sof_ipc4_gtw_attributes gw_attr;
	struct sof_ipc4_alh_multi_gtw_cfg alh_cfg;
};

/**
 * struct sof_ipc4_copier - copier config data
 * @data: IPC copier data
 * @copier_config: Copier + blob
 * @ipc_config_size: Size of copier_config
 * @available_fmt: Available audio format
 * @frame_fmt: frame format
 * @msg: message structure for copier
 * @gtw_attr: Gateway attributes for copier blob
 * @dai_type: DAI type
 * @dai_index: DAI index
 */
struct sof_ipc4_copier {
	struct sof_ipc4_copier_data data;
	u32 *copier_config;
	uint32_t ipc_config_size;
	void *ipc_config_data;
	struct sof_ipc4_available_audio_format available_fmt;
	u32 frame_fmt;
	struct sof_ipc4_msg msg;
	struct sof_ipc4_gtw_attributes *gtw_attr;
	u32 dai_type;
	int dai_index;
};

/**
 * struct sof_ipc4_ctrl_value_chan: generic channel mapped value data
 * @channel: Channel ID
 * @value: gain value
 */
struct sof_ipc4_ctrl_value_chan {
	u32 channel;
	u32 value;
};

/**
 * struct sof_ipc4_control_data - IPC data for kcontrol IO
 * @msg: message structure for kcontrol IO
 * @index: pipeline ID
 * @chanv: channel ID and value array used by volume type controls
 * @data: data for binary kcontrols
 */
struct sof_ipc4_control_data {
	struct sof_ipc4_msg msg;
	int index;

	union {
		struct sof_ipc4_ctrl_value_chan chanv[0];
		struct sof_abi_hdr data[0];
	};
};

/**
 * struct sof_ipc4_gain_data - IPC gain blob
 * @channels: Channels
 * @init_val: Initial value
 * @curve_type: Curve type
 * @reserved: reserved for future use
 * @curve_duration_l: Curve duration low part
 * @curve_duration_h: Curve duration high part
 */
struct sof_ipc4_gain_data {
	uint32_t channels;
	uint32_t init_val;
	uint32_t curve_type;
	uint32_t reserved;
	uint32_t curve_duration_l;
	uint32_t curve_duration_h;
} __aligned(8);

/**
 * struct sof_ipc4_gain - gain config data
 * @base_config: IPC base config data
 * @data: IPC gain blob
 * @available_fmt: Available audio format
 * @msg: message structure for gain
 */
struct sof_ipc4_gain {
	struct sof_ipc4_base_module_cfg base_config;
	struct sof_ipc4_gain_data data;
	struct sof_ipc4_available_audio_format available_fmt;
	struct sof_ipc4_msg msg;
};

/**
 * struct sof_ipc4_mixer - mixer config data
 * @base_config: IPC base config data
 * @available_fmt: Available audio format
 * @msg: IPC4 message struct containing header and data info
 */
struct sof_ipc4_mixer {
	struct sof_ipc4_base_module_cfg base_config;
	struct sof_ipc4_available_audio_format available_fmt;
	struct sof_ipc4_msg msg;
};

/**
 * struct sof_ipc4_src SRC config data
 * @base_config: IPC base config data
 * @sink_rate: Output rate for sink module
 * @available_fmt: Available audio format
 * @msg: IPC4 message struct containing header and data info
 */
struct sof_ipc4_src {
	struct sof_ipc4_base_module_cfg base_config;
	uint32_t sink_rate;
	struct sof_ipc4_available_audio_format available_fmt;
	struct sof_ipc4_msg msg;
};

#endif
