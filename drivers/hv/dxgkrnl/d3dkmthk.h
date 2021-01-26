// SPDX-License-Identifier: GPL-2.0

/*
 * Copyright (c) 2019, Microsoft Corporation.
 *
 * Author:
 *   Iouri Tarassov <iourit@microsoft.com>
 *
 * Dxgkrnl Graphics Port Driver user mode interface
 *
 */

#ifndef _D3DKMTHK_H
#define _D3DKMTHK_H

#include "misc.h"

#define D3DDDI_MAX_WRITTEN_PRIMARIES		16
#define D3DDDI_MAX_MPO_PRESENT_DIRTY_RECTS	0xFFF

#define D3DKMT_CREATEALLOCATION_MAX		1024
#define D3DKMT_ADAPTERS_MAX			64
#define D3DDDI_MAX_BROADCAST_CONTEXT		64
#define D3DDDI_MAX_OBJECT_WAITED_ON		32
#define D3DDDI_MAX_OBJECT_SIGNALED		32

struct d3dkmt_adapterinfo {
	struct d3dkmthandle		adapter_handle;
	struct winluid			adapter_luid;
	uint				num_sources;
	uint				present_move_regions_preferred;
};

struct d3dkmt_enumadapters2 {
	uint				num_adapters;
	struct d3dkmt_adapterinfo	*adapters;
};

struct d3dkmt_closeadapter {
	struct d3dkmthandle		adapter_handle;
};

struct d3dkmt_openadapterfromluid {
	struct winluid			adapter_luid;
	struct d3dkmthandle		adapter_handle;
};

struct d3dddi_allocationlist {
	struct d3dkmthandle		allocation;
	union {
		struct {
			uint		write_operation		:1;
			uint		do_not_retire_instance	:1;
			uint		offer_priority		:3;
			uint		reserved		:27;
		};
		uint			value;
	};
};

struct d3dddi_patchlocationlist {
	uint				allocation_index;
	union {
		struct {
			uint		slot_id:24;
			uint		reserved:8;
		};
		uint			value;
	};
	uint				driver_id;
	uint				allocation_offset;
	uint				patch_offset;
	uint				split_offset;
};

struct d3dkmt_createdeviceflags {
	uint				legacy_mode:1;
	uint				request_vSync:1;
	uint				disable_gpu_timeout:1;
	uint				reserved:29;
};

struct d3dkmt_createdevice {
	union {
		struct d3dkmthandle	adapter;
		void			*adapter_pointer;
	};
	struct d3dkmt_createdeviceflags	flags;
	struct d3dkmthandle		device;
	void				*command_buffer;
	uint				command_buffer_size;
	struct d3dddi_allocationlist	*allocation_list;
	uint				allocation_list_size;
	struct d3dddi_patchlocationlist	*patch_location_list;
	uint				patch_location_list_size;
};

struct d3dkmt_destroydevice {
	struct d3dkmthandle		device;
};

enum d3dkmt_clienthint {
	D3DKMT_CLIENTHINT_UNKNOWN	= 0,
	D3DKMT_CLIENTHINT_OPENGL	= 1,
	D3DKMT_CLIENTHINT_CDD		= 2,
	D3DKMT_CLIENTHINT_DX7		= 7,
	D3DKMT_CLIENTHINT_DX8		= 8,
	D3DKMT_CLIENTHINT_DX9		= 9,
	D3DKMT_CLIENTHINT_DX10		= 10,
};

struct d3dddi_createcontextflags {
	union {
		struct {
			uint		null_rendering:1;
			uint		initial_data:1;
			uint		disable_gpu_timeout:1;
			uint		synchronization_only:1;
			uint		hw_queue_supported:1;
			uint		reserved:27;
		};
		uint			value;
	};
};

struct d3dkmt_createcontext {
	struct d3dkmthandle		device;
	uint				node_ordinal;
	uint				engine_affinity;
	struct d3dddi_createcontextflags flags;
	void				*priv_drv_data;
	uint				priv_drv_data_size;
	enum d3dkmt_clienthint		client_hint;
	struct d3dkmthandle		context;
	void				*command_buffer;
	uint				command_buffer_size;
	struct d3dddi_allocationlist	*allocation_list;
	uint				allocation_list_size;
	struct d3dddi_patchlocationlist	*patch_location_list;
	uint				patch_location_list_size;
	u64				obsolete;
};

struct d3dkmt_destroycontext {
	struct d3dkmthandle		context;
};

struct d3dkmt_createcontextvirtual {
	struct d3dkmthandle		device;
	uint				node_ordinal;
	uint				engine_affinity;
	struct d3dddi_createcontextflags flags;
	void				*priv_drv_data;
	uint				priv_drv_data_size;
	enum d3dkmt_clienthint		client_hint;
	struct d3dkmthandle		context;
};

struct d3dddi_createhwcontextflags {
	union {
		struct {
			uint		reserved:32;
		};
		uint			value;
	};
};

struct d3dddi_createhwqueueflags {
	union {
		struct {
			uint		disable_gpu_timeout:1;
			uint		reserved:31;
		};
		uint			value;
	};
};

enum d3dddi_pagingqueue_priority {
	D3DDDI_PAGINGQUEUE_PRIORITY_BELOW_NORMAL	= -1,
	D3DDDI_PAGINGQUEUE_PRIORITY_NORMAL		= 0,
	D3DDDI_PAGINGQUEUE_PRIORITY_ABOVE_NORMAL	= 1,
};

struct d3dkmt_createpagingqueue {
	struct d3dkmthandle		device;
	enum d3dddi_pagingqueue_priority priority;
	struct d3dkmthandle		paging_queue;
	struct d3dkmthandle		sync_object;
	void				*fence_cpu_virtual_address;
	uint				physical_adapter_index;
};

struct d3dddi_destroypagingqueue {
	struct d3dkmthandle		paging_queue;
};

struct d3dkmt_renderflags {
	uint				resize_command_buffer:1;
	uint				resize_allocation_list:1;
	uint				resize_patch_location_list:1;
	uint				null_rendering:1;
	uint				present_redirected:1;
	uint				render_km:1;
	uint				render_km_readback:1;
	uint				reserved:25;
};
struct d3dkmt_render {
	union {
		struct d3dkmthandle	device;
		struct d3dkmthandle	context;
	};
	uint				command_offset;
	uint				command_length;
	uint				allocation_count;
	uint				patch_location_count;
	void				*new_command_buffer;
	uint				new_command_buffer_size;
	struct d3dddi_allocationlist	*new_allocation_list;
	uint				new_allocation_list_size;
	struct d3dddi_patchlocationlist	*new_patch_pocation_list;
	uint				new_patch_pocation_list_size;
	struct d3dkmt_renderflags	flags;
	u64				present_history_token;
	uint				broadcast_context_count;
	struct d3dkmthandle	broadcast_context[D3DDDI_MAX_BROADCAST_CONTEXT];
	uint				queued_buffer_count;
	u64				obsolete;
	void				*priv_drv_data;
	uint				priv_drv_data_size;
};

enum d3dkmt_standardallocationtype {
	D3DKMT_STANDARDALLOCATIONTYPE_EXISTINGHEAP	= 1,
};

struct d3dkmt_standardallocation_existingheap {
	u64	size;
};

struct d3dkmt_createstandardallocationflags {
	union {
		struct {
			uint		reserved:32;
		};
		uint			value;
	};
};

struct d3dkmt_createstandardallocation {
	enum d3dkmt_standardallocationtype		type;
	struct d3dkmt_standardallocation_existingheap	existing_heap_data;
	struct d3dkmt_createstandardallocationflags	flags;
};

struct d3dddi_allocationinfo2 {
	struct d3dkmthandle	allocation;
	union {
		u64		section;
		const void	*sysmem;
	};
	void			*priv_drv_data;
	uint			priv_drv_data_size;
	uint			vidpn_source_id;
	union {
		struct {
			uint	primary:1;
			uint	stereo:1;
			uint	override_priority:1;
			uint	reserved:29;
		};
		uint		value;
	} flags;
	u64			gpu_virtual_address;
	union {
		uint		priority;
		u64		unused;
	};
	u64			reserved[5];
};

struct d3dkmt_createallocationflags {
	union {
		struct {
			uint		create_resource:1;
			uint		create_shared:1;
			uint		non_secure:1;
			uint		create_protected:1;
			uint		restrict_shared_access:1;
			uint		existing_sysmem:1;
			uint		nt_security_sharing:1;
			uint		read_only:1;
			uint		create_write_combined:1;
			uint		create_cached:1;
			uint		swap_chain_back_buffer:1;
			uint		cross_adapter:1;
			uint		open_cross_adapter:1;
			uint		partial_shared_creation:1;
			uint		zeroed:1;
			uint		write_watch:1;
			uint		standard_allocation:1;
			uint		existing_section:1;
			uint		reserved:14;
		};
		uint			value;
	};
};

struct d3dkmt_createallocation {
	struct d3dkmthandle		device;
	struct d3dkmthandle		resource;
	struct d3dkmthandle		global_share;
	const void			*private_runtime_data;
	uint				private_runtime_data_size;
	union {
		struct d3dkmt_createstandardallocation *standard_allocation;
		const void		*priv_drv_data;
	};
	uint				priv_drv_data_size;
	uint				alloc_count;
	struct d3dddi_allocationinfo2	*allocation_info;
	struct d3dkmt_createallocationflags flags;
	u64				private_runtime_resource_handle;
};

struct d3dddicb_destroyallocation2flags {
	union {
		struct {
			uint		assume_not_in_use:1;
			uint		synchronous_destroy:1;
			uint		reserved:29;
			uint		system_use_only:1;
		};
		uint			value;
	};
};

struct d3dkmt_destroyallocation2 {
	struct d3dkmthandle		device;
	struct d3dkmthandle		resource;
	const struct d3dkmthandle	*allocations;
	uint				alloc_count;
	struct d3dddicb_destroyallocation2flags flags;
};

struct d3dddi_makeresident_flags {
	union {
		struct {
			uint		cant_trim_further:1;
			uint		must_succeed:1;
			uint		reserved:30;
		};
		uint			value;
	};
};

struct d3dddi_makeresident {
	struct d3dkmthandle		paging_queue;
	uint				alloc_count;
	const struct d3dkmthandle	*allocation_list;
	const uint			*priority_list;
	struct d3dddi_makeresident_flags flags;
	u64				paging_fence_value;
	u64				num_bytes_to_trim;
};

struct d3dddi_evict_flags {
	union {
		struct {
			uint		evict_only_if_necessary:1;
			uint		not_written_to:1;
			uint		reserved:30;
		};
		uint			value;
	};
};

struct d3dkmt_evict {
	struct d3dkmthandle		device;
	uint				alloc_count;
	const struct d3dkmthandle	*allocations;
	struct d3dddi_evict_flags	flags;
	u64				num_bytes_to_trim;
};

struct d3dddigpuva_protection_type {
	union {
		struct {
			u64	write:1;
			u64	execute:1;
			u64	zero:1;
			u64	no_access:1;
			u64	system_use_only:1;
			u64	reserved:59;
		};
		u64		value;
	};
};

enum d3dddi_updategpuvirtualaddress_operation_type {
	D3DDDI_UPDATEGPUVIRTUALADDRESS_MAP		= 0,
	D3DDDI_UPDATEGPUVIRTUALADDRESS_UNMAP		= 1,
	D3DDDI_UPDATEGPUVIRTUALADDRESS_COPY		= 2,
	D3DDDI_UPDATEGPUVIRTUALADDRESS_MAP_PROTECT	= 3,
};

struct d3dddi_updategpuvirtualaddress_operation {
	enum d3dddi_updategpuvirtualaddress_operation_type operation;
	union {
		struct {
			u64		base_address;
			u64		size;
			struct d3dkmthandle allocation;
			u64		allocation_offset;
			u64		allocation_size;
		} map;
		struct {
			u64		base_address;
			u64		size;
			struct d3dkmthandle allocation;
			u64		allocation_offset;
			u64		allocation_size;
			struct d3dddigpuva_protection_type protection;
			u64		driver_protection;
		} map_protect;
		struct {
			u64	base_address;
			u64	size;
			struct d3dddigpuva_protection_type protection;
		} unmap;
		struct {
			u64	source_address;
			u64	size;
			u64	dest_address;
		} copy;
	};
};

enum d3dddigpuva_reservation_type {
	D3DDDIGPUVA_RESERVE_NO_ACCESS		= 0,
	D3DDDIGPUVA_RESERVE_ZERO		= 1,
	D3DDDIGPUVA_RESERVE_NO_COMMIT		= 2
};

struct d3dkmt_updategpuvirtualaddress {
	struct d3dkmthandle			device;
	struct d3dkmthandle			context;
	struct d3dkmthandle			fence_object;
	uint					num_operations;
	struct d3dddi_updategpuvirtualaddress_operation *operations;
	uint					reserved0;
	u64					reserved1;
	u64					fence_value;
	union {
		struct {
			uint			do_not_wait:1;
			uint			reserved:31;
		};
		uint				value;
	} flags;
};

struct d3dddi_mapgpuvirtualaddress {
	struct d3dkmthandle			paging_queue;
	u64					base_address;
	u64					minimum_address;
	u64					maximum_address;
	struct d3dkmthandle			allocation;
	u64					offset_in_pages;
	u64					size_in_pages;
	struct d3dddigpuva_protection_type	protection;
	u64					driver_protection;
	uint					reserved0;
	u64					reserved1;
	u64					virtual_address;
	u64					paging_fence_value;
};

struct d3dddi_reservegpuvirtualaddress {
	struct d3dkmthandle			adapter;
	u64					base_address;
	u64					minimum_address;
	u64					maximum_address;
	u64					size;
	enum d3dddigpuva_reservation_type	reservation_type;
	u64					driver_protection;
	u64					virtual_address;
	u64					paging_fence_value;
};

struct d3dkmt_freegpuvirtualaddress {
	struct d3dkmthandle	adapter;
	u64			base_address;
	u64			size;
};

enum d3dkmt_memory_segment_group {
	D3DKMT_MEMORY_SEGMENT_GROUP_LOCAL	= 0,
	D3DKMT_MEMORY_SEGMENT_GROUP_NON_LOCAL	= 1
};

struct d3dkmt_queryvideomemoryinfo {
	u64					process;
	struct d3dkmthandle			adapter;
	enum d3dkmt_memory_segment_group	memory_segment_group;
	u64					budget;
	u64					current_usage;
	u64					current_reservation;
	u64					available_for_reservation;
	u32					physical_adapter_index;
};

enum qai_driverversion {
	KMT_DRIVERVERSION_WDDM_1_0		= 1000,
	KMT_DRIVERVERSION_WDDM_1_1_PRERELEASE	= 1102,
	KMT_DRIVERVERSION_WDDM_1_1		= 1105,
	KMT_DRIVERVERSION_WDDM_1_2		= 1200,
	KMT_DRIVERVERSION_WDDM_1_3		= 1300,
	KMT_DRIVERVERSION_WDDM_2_0		= 2000,
	KMT_DRIVERVERSION_WDDM_2_1		= 2100,
	KMT_DRIVERVERSION_WDDM_2_2		= 2200,
	KMT_DRIVERVERSION_WDDM_2_3		= 2300,
	KMT_DRIVERVERSION_WDDM_2_4		= 2400,
	KMT_DRIVERVERSION_WDDM_2_5		= 2500,
	KMT_DRIVERVERSION_WDDM_2_6		= 2600,
	KMT_DRIVERVERSION_WDDM_2_7		= 2700
};

struct d3dkmt_adaptertype {
	union {
		struct {
			uint			render_supported:1;
			uint			display_supported:1;
			uint			software_device:1;
			uint			post_device:1;
			uint			hybrid_discrete:1;
			uint			hybrid_integrated:1;
			uint			indirect_display_device:1;
			uint			paravirtualized:1;
			uint			acg_supported:1;
			uint			support_set_timings_from_vidpn:1;
			uint			detachable:1;
			uint			compute_only:1;
			uint			prototype:1;
			uint			reserved:19;
		};
		uint				value;
	};
};

enum kmtqueryadapterinfotype {
	KMTQAITYPE_UMDRIVERPRIVATE				= 0,
	KMTQAITYPE_UMDRIVERNAME					= 1,
	KMTQAITYPE_UMOPENGLINFO					= 2,
	KMTQAITYPE_GETSEGMENTSIZE				= 3,
	KMTQAITYPE_ADAPTERGUID					= 4,
	KMTQAITYPE_FLIPQUEUEINFO				= 5,
	KMTQAITYPE_ADAPTERADDRESS				= 6,
	KMTQAITYPE_SETWORKINGSETINFO				= 7,
	KMTQAITYPE_ADAPTERREGISTRYINFO				= 8,
	KMTQAITYPE_CURRENTDISPLAYMODE				= 9,
	KMTQAITYPE_MODELIST					= 10,
	KMTQAITYPE_CHECKDRIVERUPDATESTATUS			= 11,
	KMTQAITYPE_VIRTUALADDRESSINFO				= 12,
	KMTQAITYPE_DRIVERVERSION				= 13,
	KMTQAITYPE_ADAPTERTYPE					= 15,
	KMTQAITYPE_OUTPUTDUPLCONTEXTSCOUNT			= 16,
	KMTQAITYPE_WDDM_1_2_CAPS				= 17,
	KMTQAITYPE_UMD_DRIVER_VERSION				= 18,
	KMTQAITYPE_DIRECTFLIP_SUPPORT				= 19,
	KMTQAITYPE_MULTIPLANEOVERLAY_SUPPORT			= 20,
	KMTQAITYPE_DLIST_DRIVER_NAME				= 21,
	KMTQAITYPE_WDDM_1_3_CAPS				= 22,
	KMTQAITYPE_MULTIPLANEOVERLAY_HUD_SUPPORT		= 23,
	KMTQAITYPE_WDDM_2_0_CAPS				= 24,
	KMTQAITYPE_NODEMETADATA					= 25,
	KMTQAITYPE_CPDRIVERNAME					= 26,
	KMTQAITYPE_XBOX						= 27,
	KMTQAITYPE_INDEPENDENTFLIP_SUPPORT			= 28,
	KMTQAITYPE_MIRACASTCOMPANIONDRIVERNAME			= 29,
	KMTQAITYPE_PHYSICALADAPTERCOUNT				= 30,
	KMTQAITYPE_PHYSICALADAPTERDEVICEIDS			= 31,
	KMTQAITYPE_DRIVERCAPS_EXT				= 32,
	KMTQAITYPE_QUERY_MIRACAST_DRIVER_TYPE			= 33,
	KMTQAITYPE_QUERY_GPUMMU_CAPS				= 34,
	KMTQAITYPE_QUERY_MULTIPLANEOVERLAY_DECODE_SUPPORT	= 35,
	KMTQAITYPE_QUERY_HW_PROTECTION_TEARDOWN_COUNT		= 36,
	KMTQAITYPE_QUERY_ISBADDRIVERFORHWPROTECTIONDISABLED	= 37,
	KMTQAITYPE_MULTIPLANEOVERLAY_SECONDARY_SUPPORT		= 38,
	KMTQAITYPE_INDEPENDENTFLIP_SECONDARY_SUPPORT		= 39,
	KMTQAITYPE_PANELFITTER_SUPPORT				= 40,
	KMTQAITYPE_PHYSICALADAPTERPNPKEY			= 41,
	KMTQAITYPE_GETSEGMENTGROUPSIZE				= 42,
	KMTQAITYPE_MPO3DDI_SUPPORT				= 43,
	KMTQAITYPE_HWDRM_SUPPORT				= 44,
	KMTQAITYPE_MPOKERNELCAPS_SUPPORT			= 45,
	KMTQAITYPE_MULTIPLANEOVERLAY_STRETCH_SUPPORT		= 46,
	KMTQAITYPE_GET_DEVICE_VIDPN_OWNERSHIP_INFO		= 47,
	KMTQAITYPE_QUERYREGISTRY				= 48,
	KMTQAITYPE_KMD_DRIVER_VERSION				= 49,
	KMTQAITYPE_BLOCKLIST_KERNEL				= 50,
	KMTQAITYPE_BLOCKLIST_RUNTIME				= 51,
	KMTQAITYPE_ADAPTERGUID_RENDER				= 52,
	KMTQAITYPE_ADAPTERADDRESS_RENDER			= 53,
	KMTQAITYPE_ADAPTERREGISTRYINFO_RENDER			= 54,
	KMTQAITYPE_CHECKDRIVERUPDATESTATUS_RENDER		= 55,
	KMTQAITYPE_DRIVERVERSION_RENDER				= 56,
	KMTQAITYPE_ADAPTERTYPE_RENDER				= 57,
	KMTQAITYPE_WDDM_1_2_CAPS_RENDER				= 58,
	KMTQAITYPE_WDDM_1_3_CAPS_RENDER				= 59,
	KMTQAITYPE_QUERY_ADAPTER_UNIQUE_GUID			= 60,
	KMTQAITYPE_NODEPERFDATA					= 61,
	KMTQAITYPE_ADAPTERPERFDATA				= 62,
	KMTQAITYPE_ADAPTERPERFDATA_CAPS				= 63,
	KMTQUITYPE_GPUVERSION					= 64,
	KMTQAITYPE_DRIVER_DESCRIPTION				= 65,
	KMTQAITYPE_DRIVER_DESCRIPTION_RENDER			= 66,
	KMTQAITYPE_SCANOUT_CAPS					= 67,
	KMTQAITYPE_PARAVIRTUALIZATION_RENDER			= 68,
};

struct d3dkmt_queryadapterinfo {
	struct d3dkmthandle		adapter;
	enum kmtqueryadapterinfotype	type;
	void				*private_data;
	uint				private_data_size;
};

enum d3dkmt_escapetype {
	D3DKMT_ESCAPE_DRIVERPRIVATE	= 0,
	D3DKMT_ESCAPE_VIDMM		= 1,
	D3DKMT_ESCAPE_VIDSCH		= 3,
	D3DKMT_ESCAPE_DEVICE		= 4,
	D3DKMT_ESCAPE_DRT_TEST		= 8,
};

enum d3dkmt_drt_test_command {
	D3DKMT_DRT_TEST_COMMAND_HANDLETABLE = 39,
};

struct d3dkmt_drt_escape_head {
	uint				signature;
	uint				buffer_size;
	enum d3dkmt_drt_test_command	command;
};

enum d3dkmt_ht_command {
	D3DKMT_HT_COMMAND_ALLOC,
	D3DKMT_HT_COMMAND_FREE,
	D3DKMT_HT_COMMAND_ASSIGN,
	D3DKMT_HT_COMMAND_GET,
	D3DKMT_HT_COMMAND_DESTROY,
};

struct d3dkmt_ht_desc {
	struct d3dkmt_drt_escape_head	head;
	enum d3dkmt_ht_command		command;
	uint				index;
	struct d3dkmthandle		handle;
	uint				object_type;
	void				*object;
};

struct d3dddi_escapeflags {
	union {
		struct {
			uint		hardware_access:1;
			uint		device_status_query:1;
			uint		change_frame_latency:1;
			uint		no_adapter_synchronization:1;
			uint		reserved:1;
			uint		virtual_machine_data:1;
			uint		driver_known_escape:1;
			uint		driver_common_escape:1;
			uint		reserved2:24;
		};
		uint			value;
	};
};

struct d3dkmt_escape {
	struct d3dkmthandle		adapter;
	struct d3dkmthandle		device;
	enum d3dkmt_escapetype		type;
	struct d3dddi_escapeflags	flags;
	void				*priv_drv_data;
	uint				priv_drv_data_size;
	struct d3dkmthandle		context;
};

enum dxgk_render_pipeline_stage {
	DXGK_RENDER_PIPELINE_STAGE_UNKNOWN		= 0,
	DXGK_RENDER_PIPELINE_STAGE_INPUT_ASSEMBLER	= 1,
	DXGK_RENDER_PIPELINE_STAGE_VERTEX_SHADER	= 2,
	DXGK_RENDER_PIPELINE_STAGE_GEOMETRY_SHADER	= 3,
	DXGK_RENDER_PIPELINE_STAGE_STREAM_OUTPUT	= 4,
	DXGK_RENDER_PIPELINE_STAGE_RASTERIZER		= 5,
	DXGK_RENDER_PIPELINE_STAGE_PIXEL_SHADER		= 6,
	DXGK_RENDER_PIPELINE_STAGE_OUTPUT_MERGER	= 7,
};

enum dxgk_page_fault_flags {
	DXGK_PAGE_FAULT_WRITE			= 0x1,
	DXGK_PAGE_FAULT_FENCE_INVALID		= 0x2,
	DXGK_PAGE_FAULT_ADAPTER_RESET_REQUIRED	= 0x4,
	DXGK_PAGE_FAULT_ENGINE_RESET_REQUIRED	= 0x8,
	DXGK_PAGE_FAULT_FATAL_HARDWARE_ERROR	= 0x10,
	DXGK_PAGE_FAULT_IOMMU			= 0x20,
	DXGK_PAGE_FAULT_HW_CONTEXT_VALID	= 0x40,
	DXGK_PAGE_FAULT_PROCESS_HANDLE_VALID	= 0x80,
};

enum dxgk_general_error_code {
	DXGK_GENERAL_ERROR_PAGE_FAULT		= 0,
	DXGK_GENERAL_ERROR_INVALID_INSTRUCTION	= 1,
};

struct dxgk_fault_error_code {
	union {
		struct {
			uint	is_device_specific_code:1;
			enum dxgk_general_error_code general_error_code:31;
		};
		struct {
			uint	is_device_specific_code_reserved_bit:1;
			uint	device_specific_code:31;
		};
	};
};

enum d3dkmt_deviceexecution_state {
	D3DKMT_DEVICEEXECUTION_ACTIVE			= 1,
	D3DKMT_DEVICEEXECUTION_RESET			= 2,
	D3DKMT_DEVICEEXECUTION_HUNG			= 3,
	D3DKMT_DEVICEEXECUTION_STOPPED			= 4,
	D3DKMT_DEVICEEXECUTION_ERROR_OUTOFMEMORY	= 5,
	D3DKMT_DEVICEEXECUTION_ERROR_DMAFAULT		= 6,
	D3DKMT_DEVICEEXECUTION_ERROR_DMAPAGEFAULT	= 7,
};

struct d3dkmt_devicereset_state {
	union {
		struct {
			uint	desktop_switched:1;
			uint	reserved:31;
		};
		uint		value;
	};
};

struct d3dkmt_present_stats {
	uint		present_count;
	uint		present_refresh_count;
	uint		sync_refresh_count;
	u64		sync_qpc_time;
	u64		sync_gpu_time;
};

struct d3dkmt_devicepresent_state {
	uint				vidpn_source_id;
	struct d3dkmt_present_stats	present_stats;
};

struct d3dkmt_present_stats_dwm {
	uint	present_count;
	uint	present_refresh_count;
	u64	present_qpc_time;
	uint	sync_refresh_count;
	u64	sync_qpc_time;
	uint	custom_present_duration;
};

struct d3dkmt_devicepagefault_state {
	u64				faulted_primitive_api_sequence_number;
	enum dxgk_render_pipeline_stage	faulted_pipeline_stage;
	uint				faulted_bind_table_entry;
	enum dxgk_page_fault_flags	page_fault_flags;
	struct dxgk_fault_error_code	fault_error_code;
	u64				faulted_virtual_address;
};

struct d3dkmt_devicepresent_state_dwm {
	uint				vidpn_source_id;
	struct d3dkmt_present_stats_dwm	present_stats;
};

struct d3dkmt_devicepresent_queue_state {
	uint	vidpn_source_id;
	bool	bQueuedPresentLimitReached;
};

enum d3dkmt_devicestate_type {
	D3DKMT_DEVICESTATE_EXECUTION		= 1,
	D3DKMT_DEVICESTATE_PRESENT		= 2,
	D3DKMT_DEVICESTATE_RESET		= 3,
	D3DKMT_DEVICESTATE_PRESENT_DWM		= 4,
	D3DKMT_DEVICESTATE_PAGE_FAULT		= 5,
	D3DKMT_DEVICESTATE_PRESENT_QUEUE	= 6,
};

struct d3dkmt_getdevicestate {
	struct d3dkmthandle				device;
	enum d3dkmt_devicestate_type			state_type;
	union {
		enum d3dkmt_deviceexecution_state	execution_state;
		struct d3dkmt_devicepresent_state	present_state;
		struct d3dkmt_devicereset_state		reset_state;
		struct d3dkmt_devicepresent_state_dwm	present_state_dwm;
		struct d3dkmt_devicepagefault_state	page_fault_state;
		struct d3dkmt_devicepresent_queue_state	present_queue_state;
	};
};

enum d3dkmdt_gdisurfacetype {
	D3DKMDT_GDISURFACE_INVALID				= 0,
	D3DKMDT_GDISURFACE_TEXTURE				= 1,
	D3DKMDT_GDISURFACE_STAGING_CPUVISIBLE			= 2,
	D3DKMDT_GDISURFACE_STAGING				= 3,
	D3DKMDT_GDISURFACE_LOOKUPTABLE				= 4,
	D3DKMDT_GDISURFACE_EXISTINGSYSMEM			= 5,
	D3DKMDT_GDISURFACE_TEXTURE_CPUVISIBLE			= 6,
	D3DKMDT_GDISURFACE_TEXTURE_CROSSADAPTER			= 7,
	D3DKMDT_GDISURFACE_TEXTURE_CPUVISIBLE_CROSSADAPTER	= 8,
};

struct d3dddi_rational {
	uint	numerator;
	uint	denominator;
};

enum d3dddiformat {
	D3DDDIFMT_UNKNOWN = 0,
};

struct d3dkmdt_gdisurfacedata {
	uint				width;
	uint				height;
	uint				format;
	enum d3dkmdt_gdisurfacetype 	type;
	uint				flags;
	uint				pitch;
};

struct d3dkmtd_stagingsurfacedata {
	uint	width;
	uint	height;
	uint	pitch;
};

struct d3dkmdt_sharedprimarysurfacedata {
	uint			width;
	uint			height;
	enum d3dddiformat	format;
	struct d3dddi_rational	refresh_rate;
	uint			vidpn_source_id;
};

struct d3dkmdt_shadowsurfacedata {
	uint			width;
	uint			height;
	enum d3dddiformat	format;
	uint			pitch;
};

enum d3dkmdt_standardallocationtype {
	D3DKMDT_STANDARDALLOCATION_SHAREDPRIMARYSURFACE	= 1,
	D3DKMDT_STANDARDALLOCATION_SHADOWSURFACE	= 2,
	D3DKMDT_STANDARDALLOCATION_STAGINGSURFACE	= 3,
	D3DKMDT_STANDARDALLOCATION_GDISURFACE		= 4,
};

struct d3dddi_synchronizationobject_flags {
	union {
		struct {
			uint	shared:1;
			uint	nt_security_sharing:1;
			uint	cross_adapter:1;
			uint	top_of_pipeline:1;
			uint	no_signal:1;
			uint	no_wait:1;
			uint	no_signal_max_value_on_tdr:1;
			uint	no_gpu_access:1;
			uint	reserved:23;
		};
		uint		value;
	};
};

enum d3dddi_synchronizationobject_type {
	D3DDDI_SYNCHRONIZATION_MUTEX		= 1,
	D3DDDI_SEMAPHORE			= 2,
	D3DDDI_FENCE				= 3,
	D3DDDI_CPU_NOTIFICATION			= 4,
	D3DDDI_MONITORED_FENCE			= 5,
	D3DDDI_PERIODIC_MONITORED_FENCE		= 6,
	D3DDDI_SYNCHRONIZATION_TYPE_LIMIT
};

struct d3dddi_synchronizationobjectinfo2 {
	enum d3dddi_synchronizationobject_type	type;
	struct d3dddi_synchronizationobject_flags flags;
	union {
		struct {
			uint	initial_state;
		} synchronization_mutex;

		struct {
			uint			max_count;
			uint			initial_count;
		} semaphore;

		struct {
			u64 		fence_value;
		} fence;

		struct {
			u64		event;
		} cpu_notification;

		struct {
			u64	initial_fence_value;
			void	*fence_cpu_virtual_address;
			u64	fence_gpu_virtual_address;
			uint	engine_affinity;
		} monitored_fence;

		struct periodic_monitored_fence_t {
			struct d3dkmthandle	adapter;
			uint			vidpn_target_id;
			u64			time;
			void			*fence_cpu_virtual_address;
			u64			fence_gpu_virtual_address;
			uint			engine_affinity;
		} periodic_monitored_fence;

		struct {
			u64	reserved[8];
		} reserved;
	};
	struct d3dkmthandle			shared_handle;
};

struct d3dkmt_createsynchronizationobject2 {
	struct d3dkmthandle			device;
	struct d3dddi_synchronizationobjectinfo2 info;
	struct d3dkmthandle			sync_object;
};

struct d3dkmt_waitforsynchronizationobject2 {
	struct d3dkmthandle	context;
	uint			object_count;
	struct d3dkmthandle	object_array[D3DDDI_MAX_OBJECT_WAITED_ON];
	union {
		struct {
			u64	fence_value;
		} fence;
		u64		reserved[8];
	};
};

struct d3dddicb_signalflags {
	union {
		struct {
			uint			signal_at_submission:1;
			uint			enqueue_cpu_event:1;
			uint			allow_fence_rewind:1;
			uint			reserved:28;
			uint			DXGK_SIGNAL_FLAG_INTERNAL0:1;
		};
		uint				value;
	};
};

struct d3dkmt_signalsynchronizationobject2 {
	struct d3dkmthandle		context;
	uint				object_count;
	struct d3dkmthandle	object_array[D3DDDI_MAX_OBJECT_SIGNALED];
	struct d3dddicb_signalflags	flags;
	uint				context_count;
	struct d3dkmthandle		contexts[D3DDDI_MAX_BROADCAST_CONTEXT];
	union {
		struct {
			u64		fence_value;
		} fence;
		u64			cpu_event_handle;
		u64			reserved[8];
	};
};

struct d3dddi_waitforsynchronizationobjectfromcpu_flags {
	union {
		struct {
			uint			wait_any:1;
			uint			reserved:31;
		};
		uint				value;
	};
};

struct d3dkmt_waitforsynchronizationobjectfromcpu {
	struct d3dkmthandle			device;
	uint					object_count;
	struct d3dkmthandle			*objects;
	u64					*fence_values;
	u64					async_event;
	struct d3dddi_waitforsynchronizationobjectfromcpu_flags flags;
};

struct d3dkmt_signalsynchronizationobjectfromcpu {
	struct d3dkmthandle			device;
	uint					object_count;
	struct d3dkmthandle			*objects;
	u64					*fence_values;
	struct d3dddicb_signalflags		flags;
};

struct d3dkmt_waitforsynchronizationobjectfromgpu {
	struct d3dkmthandle			context;
	uint					object_count;
	struct d3dkmthandle			*objects;
	union {
		u64			*monitored_fence_values;
		u64			fence_value;
		u64			reserved[8];
	};
};

struct d3dkmt_signalsynchronizationobjectfromgpu {
	struct d3dkmthandle		context;
	uint				object_count;
	struct d3dkmthandle		*objects;
	union {
		u64			*monitored_fence_values;
		u64			reserved[8];
	};
};

struct d3dkmt_signalsynchronizationobjectfromgpu2 {
	uint				object_count;
	struct d3dkmthandle		*objects;
	struct d3dddicb_signalflags	flags;
	uint				context_count;
	struct d3dkmthandle		*contexts;
	union {
		u64			fence_value;
		u64			cpu_event_handle;
		u64			*monitored_fence_values;
		u64			reserved[8];
	};
};

struct d3dkmt_destroysynchronizationobject {
	struct d3dkmthandle	sync_object;
};

struct d3dkmt_opensynchronizationobject {
	struct d3dkmthandle	shared_handle;
	struct d3dkmthandle	sync_object;
	u64			reserved[8];
};

struct d3dkmt_submitcommandflags {
	uint					null_rendering:1;
	uint					present_redirected:1;
	uint					reserved:30;
};

struct d3dkmt_submitcommand {
	u64					command_buffer;
	uint					command_length;
	struct d3dkmt_submitcommandflags	flags;
	u64					present_history_token;
	uint					broadcast_context_count;
	struct d3dkmthandle	broadcast_context[D3DDDI_MAX_BROADCAST_CONTEXT];
	void					*priv_drv_data;
	uint					priv_drv_data_size;
	uint					num_primaries;
	struct d3dkmthandle	written_primaries[D3DDDI_MAX_WRITTEN_PRIMARIES];
	uint					num_history_buffers;
	struct d3dkmthandle			*history_buffer_array;
};

struct d3dkmt_submitcommandtohwqueue {
	struct d3dkmthandle			hwqueue;
	u64					hwqueue_progress_fence_id;
	u64					command_buffer;
	uint					command_length;
	uint					priv_drv_data_size;
	void					*priv_drv_data;
	uint					num_primaries;
	struct d3dkmthandle			*written_primaries;
};

struct d3dkmt_setcontextschedulingpriority {
	struct d3dkmthandle			context;
	int					priority;
};

struct d3dkmt_setcontextinprocessschedulingpriority {
	struct d3dkmthandle			context;
	int					priority;
};

struct d3dkmt_getcontextschedulingpriority {
	struct d3dkmthandle			context;
	int					priority;
};

struct d3dkmt_getcontextinprocessschedulingpriority {
	struct d3dkmthandle			context;
	int					priority;
};

struct d3dkmt_setallocationpriority {
	struct d3dkmthandle		device;
	struct d3dkmthandle		resource;
	const struct d3dkmthandle	*allocation_list;
	uint				allocation_count;
	const uint			*priorities;
};

struct d3dkmt_getallocationpriority {
	struct d3dkmthandle		device;
	struct d3dkmthandle		resource;
	const struct d3dkmthandle	*allocation_list;
	uint				allocation_count;
	uint				*priorities;
};

enum d3dkmt_allocationresidencystatus {
	D3DKMT_ALLOCATIONRESIDENCYSTATUS_RESIDENTINGPUMEMORY	= 1,
	D3DKMT_ALLOCATIONRESIDENCYSTATUS_RESIDENTINSHAREDMEMORY	= 2,
	D3DKMT_ALLOCATIONRESIDENCYSTATUS_NOTRESIDENT		= 3,
};

struct d3dkmt_queryallocationresidency {
	struct d3dkmthandle			device;
	struct d3dkmthandle			resource;
	struct d3dkmthandle			*allocations;
	uint					allocation_count;
	enum d3dkmt_allocationresidencystatus	*residency_status;
};

struct D3DDDICB_LOCK2FLAGS {
	union {
		struct {
			uint			reserved:32;
		};
		uint				value;
	};
};

struct d3dkmt_lock2 {
	struct d3dkmthandle			device;
	struct d3dkmthandle			allocation;
	struct D3DDDICB_LOCK2FLAGS		flags;
	void					*data;
};

struct d3dkmt_unlock2 {
	struct d3dkmthandle			device;
	struct d3dkmthandle			allocation;
};

enum D3DKMT_DEVICE_ERROR_REASON {
	D3DKMT_DEVICE_ERROR_REASON_GENERIC	= 0x80000000,
	D3DKMT_DEVICE_ERROR_REASON_DRIVER_ERROR	= 0x80000006,
};

struct d3dkmt_markdeviceaserror {
	struct d3dkmthandle			device;
	enum D3DKMT_DEVICE_ERROR_REASON		reason;
};

struct D3DDDI_UPDATEALLOCPROPERTY_FLAGS {
	union {
		struct {
			uint			accessed_physically:1;
			uint			reserved:31;
		};
		uint				value;
	};
};

struct D3DDDI_SEGMENTPREFERENCE {
	union {
		struct {
			uint			segment_id0:5;
			uint			direction0:1;
			uint			segment_id1:5;
			uint			direction1:1;
			uint			segment_id2:5;
			uint			direction2:1;
			uint			segment_id3:5;
			uint			direction3:1;
			uint			segment_id4:5;
			uint			direction4:1;
			uint			reserved:2;
		};
		uint				value;
	};
};

struct d3dddi_updateallocproperty {
	struct d3dkmthandle			paging_queue;
	struct d3dkmthandle			allocation;
	uint					supported_segment_set;
	struct D3DDDI_SEGMENTPREFERENCE		preferred_segment;
	struct D3DDDI_UPDATEALLOCPROPERTY_FLAGS	flags;
	u64					paging_fence_value;
	union {
		struct {
			uint			set_accessed_physically:1;
			uint			set_supported_segmentSet:1;
			uint			set_preferred_segment:1;
			uint			reserved:29;
		};
		uint				property_mask_value;
	};
};

enum d3dkmt_offer_priority {
	D3DKMT_OFFER_PRIORITY_LOW	= 1,
	D3DKMT_OFFER_PRIORITY_NORMAL	= 2,
	D3DKMT_OFFER_PRIORITY_HIGH	= 3,
	D3DKMT_OFFER_PRIORITY_AUTO	= 4,
};

struct d3dkmt_offer_flags {
	union {
		struct {
			uint	offer_immediately:1;
			uint	allow_decommit:1;
			uint	reserved:30;
		};
		uint		value;
	};
};

struct d3dkmt_offerallocations {
	struct d3dkmthandle		device;
	struct d3dkmthandle		*resources;
	const struct d3dkmthandle	*allocations;
	uint				allocation_count;
	enum d3dkmt_offer_priority	priority;
	struct d3dkmt_offer_flags	flags;
};

enum d3dddi_reclaim_result {
	D3DDDI_RECLAIM_RESULT_OK		= 0,
	D3DDDI_RECLAIM_RESULT_DISCARDED		= 1,
	D3DDDI_RECLAIM_RESULT_NOT_COMMITTED	= 2,
};

struct d3dkmt_reclaimallocations2 {
	struct d3dkmthandle	paging_queue;
	uint			allocation_count;
	struct d3dkmthandle	*resources;
	struct d3dkmthandle	*allocations;
	union {
		uint	*discarded;
		enum d3dddi_reclaim_result *results;
	};
	u64			paging_fence_value;
};

struct d3dkmt_changevideomemoryreservation {
	u64			process;
	struct d3dkmthandle	adapter;
	enum d3dkmt_memory_segment_group memory_segment_group;
	u64			reservation;
	uint			physical_adapter_index;
};

struct d3dkmt_createhwcontext {
	struct d3dkmthandle	device;
	uint			node_ordinal;
	uint			engine_affinity;
	struct d3dddi_createhwcontextflags flags;
	uint			priv_drv_data_size;
	void			*priv_drv_data;
	struct d3dkmthandle	context;
};

struct d3dkmt_destroyhwcontext {
	struct d3dkmthandle	context;
};

struct d3dkmt_createhwqueue {
	struct d3dkmthandle	context;
	struct d3dddi_createhwqueueflags flags;
	uint			priv_drv_data_size;
	void			*priv_drv_data;
	struct d3dkmthandle	queue;
	struct d3dkmthandle	queue_progress_fence;
	void			*queue_progress_fence_cpu_va;
	u64			queue_progress_fence_gpu_va;
};

struct d3dkmt_destroyhwqueue {
	struct d3dkmthandle	queue;
};

struct d3dkmt_submitwaitforsyncobjectstohwqueue {
	struct d3dkmthandle	hwqueue;
	uint			object_count;
	struct d3dkmthandle	*objects;
	u64			*fence_values;
};

struct d3dkmt_submitsignalsyncobjectstohwqueue {
	struct d3dddicb_signalflags flags;
	uint			hwqueue_count;
	struct d3dkmthandle	*hwqueues;
	uint			object_count;
	struct d3dkmthandle	*objects;
	u64			*fence_values;
};

struct dxgk_gpuclockdata_flags {
	union {
		struct {
			uint	context_management_processor:1;
			uint	reserved:31;
		};
		uint		value;
	};
};

struct dxgk_gpuclockdata {
	u64				gpu_frequency;
	u64				gpu_clock_counter;
	u64				cpu_clock_counter;
	struct dxgk_gpuclockdata_flags	flags;
} __packed;

struct d3dkmt_queryclockcalibration {
	struct d3dkmthandle	adapter;
	uint			node_ordinal;
	uint			physical_adapter_index;
	struct dxgk_gpuclockdata clock_data;
};

struct d3dkmt_flushheaptransitions {
	struct d3dkmthandle	adapter;
};

struct d3dkmt_getsharedresourceadapterluid {
	struct d3dkmthandle	global_share;
	u64			handle;
	struct winluid		adapter_luid;
};

struct d3dkmt_invalidatecache {
	struct d3dkmthandle	device;
	struct d3dkmthandle	allocation;
	u64			offset;
	u64			length;
};

struct d3dddi_openallocationinfo2 {
	struct d3dkmthandle	allocation;
	void			*priv_drv_data;
	uint			priv_drv_data_size;
	u64			gpu_va;
	u64			reserved[6];
};

struct d3dkmt_opensyncobjectfromnthandle {
	u64			nt_handle;
	struct d3dkmthandle	sync_object;
};

struct d3dkmt_opensyncobjectfromnthandle2 {
	u64			nt_handle;
	struct d3dkmthandle	device;
	struct d3dddi_synchronizationobject_flags flags;
	struct d3dkmthandle	sync_object;
	union {
		struct {
			void	*fence_value_cpu_va;
			u64	fence_value_gpu_va;
			uint	engine_affinity;
		} monitored_fence;
		u64	reserved[8];
	};
};

struct d3dkmt_openresource {
	struct d3dkmthandle	device;
	struct d3dkmthandle	global_share;
	uint			allocation_count;
	struct d3dddi_openallocationinfo2 *open_alloc_info;
	void			*private_runtime_data;
	int 			private_runtime_data_size;
	void			*resource_priv_drv_data;
	uint			resource_priv_drv_data_size;
	void			*total_priv_drv_data;
	uint			total_priv_drv_data_size;
	struct d3dkmthandle	resource;
};

struct d3dkmt_openresourcefromnthandle {
	struct d3dkmthandle	device;
	u64			nt_handle;
	uint			allocation_count;
	struct d3dddi_openallocationinfo2 *open_alloc_info;
	int 			private_runtime_data_size;
	void			*private_runtime_data;
	uint			resource_priv_drv_data_size;
	void			*resource_priv_drv_data;
	uint			total_priv_drv_data_size;
	void			*total_priv_drv_data;
	struct d3dkmthandle	resource;
	struct d3dkmthandle	keyed_mutex;
	void			*keyed_mutex_private_data;
	uint			keyed_mutex_private_data_size;
	struct d3dkmthandle	sync_object;
};

struct d3dkmt_queryresourceinfofromnthandle {
	struct d3dkmthandle	device;
	u64			nt_handle;
	void			*private_runtime_data;
	uint			private_runtime_data_size;
	uint			total_priv_drv_data_size;
	uint			resource_priv_drv_data_size;
	uint			allocation_count;
};

struct d3dkmt_queryresourceinfo {
	struct d3dkmthandle	device;
	struct d3dkmthandle	global_share;
	void			*private_runtime_data;
	uint			private_runtime_data_size;
	uint			total_priv_drv_data_size;
	uint			resource_priv_drv_data_size;
	uint			allocation_count;
};

struct d3dkmt_shareobjects {
	uint			object_count;
	const struct d3dkmthandle *objects;	/* per-process DXG handle */
	void			*object_attr;	/* security attributes */
	uint			desired_access;
	u64			*shared_handle;	/* output file descriptor */
};

union d3dkmt_enumadapters_filter {
	struct {
		u64	include_compute_only:1;
		u64	include_display_only:1;
		u64	reserved:62;
	};
	u64		value;
};

struct d3dkmt_enumadapters3 {
	union d3dkmt_enumadapters_filter	filter;
	uint					adapter_count;
	struct d3dkmt_adapterinfo		*adapters;
};

enum d3dkmt_querystatistics_type
{
	D3DKMT_QUERYSTATISTICS_ADAPTER                = 0,
	D3DKMT_QUERYSTATISTICS_PROCESS                = 1,
	D3DKMT_QUERYSTATISTICS_PROCESS_ADAPTER        = 2,
	D3DKMT_QUERYSTATISTICS_SEGMENT                = 3,
	D3DKMT_QUERYSTATISTICS_PROCESS_SEGMENT        = 4,
	D3DKMT_QUERYSTATISTICS_NODE                   = 5,
	D3DKMT_QUERYSTATISTICS_PROCESS_NODE           = 6,
	D3DKMT_QUERYSTATISTICS_VIDPNSOURCE            = 7,
	D3DKMT_QUERYSTATISTICS_PROCESS_VIDPNSOURCE    = 8,
	D3DKMT_QUERYSTATISTICS_PROCESS_SEGMENT_GROUP  = 9,
	D3DKMT_QUERYSTATISTICS_PHYSICAL_ADAPTER       = 10,
};

struct d3dkmt_querystatistics_result {
	char size[0x308];
};

struct d3dkmt_querystatistics {
	union {
		struct {
			enum d3dkmt_querystatistics_type	type;
			struct winluid				adapter_luid;
			u64					process;
			struct d3dkmt_querystatistics_result	result;
		};
		char size[0x328];
	};
};

/*
 * Dxgkrnl Graphics Port Driver ioctl definitions
 *
 */

#define LX_DXOPENADAPTERFROMLUID	\
	_IOWR(0x47, 0x01, struct d3dkmt_openadapterfromluid)
#define LX_DXCREATEDEVICE		\
	_IOWR(0x47, 0x02, struct d3dkmt_createdevice)
#define LX_DXCREATECONTEXT		\
	_IOWR(0x47, 0x03, struct d3dkmt_createcontext)
#define LX_DXCREATECONTEXTVIRTUAL	\
	_IOWR(0x47, 0x04, struct d3dkmt_createcontextvirtual)
#define LX_DXDESTROYCONTEXT		\
	_IOWR(0x47, 0x05, struct d3dkmt_destroycontext)
#define LX_DXCREATEALLOCATION		\
	_IOWR(0x47, 0x06, struct d3dkmt_createallocation)
#define LX_DXCREATEPAGINGQUEUE		\
	_IOWR(0x47, 0x07, struct d3dkmt_createpagingqueue)
#define LX_DXRESERVEGPUVIRTUALADDRESS	\
	_IOWR(0x47, 0x08, struct d3dddi_reservegpuvirtualaddress)
#define LX_DXQUERYADAPTERINFO		\
	_IOWR(0x47, 0x09, struct d3dkmt_queryadapterinfo)
#define LX_DXQUERYVIDEOMEMORYINFO	\
	_IOWR(0x47, 0x0a, struct d3dkmt_queryvideomemoryinfo)
#define LX_DXMAKERESIDENT		\
	_IOWR(0x47, 0x0b, struct d3dddi_makeresident)
#define LX_DXMAPGPUVIRTUALADDRESS	\
	_IOWR(0x47, 0x0c, struct d3dddi_mapgpuvirtualaddress)
#define LX_DXESCAPE			\
	_IOWR(0x47, 0x0d, struct d3dkmt_escape)
#define LX_DXGETDEVICESTATE		\
	_IOWR(0x47, 0x0e, struct d3dkmt_getdevicestate)
#define LX_DXSUBMITCOMMAND		\
	_IOWR(0x47, 0x0f, struct d3dkmt_submitcommand)
#define LX_DXCREATESYNCHRONIZATIONOBJECT \
	_IOWR(0x47, 0x10, struct d3dkmt_createsynchronizationobject2)
#define LX_DXSIGNALSYNCHRONIZATIONOBJECT \
	_IOWR(0x47, 0x11, struct d3dkmt_signalsynchronizationobject2)
#define LX_DXWAITFORSYNCHRONIZATIONOBJECT \
	_IOWR(0x47, 0x12, struct d3dkmt_waitforsynchronizationobject2)
#define LX_DXDESTROYALLOCATION2		\
	_IOWR(0x47, 0x13, struct d3dkmt_destroyallocation2)
#define LX_DXENUMADAPTERS2		\
	_IOWR(0x47, 0x14, struct d3dkmt_enumadapters2)
#define LX_DXCLOSEADAPTER		\
	_IOWR(0x47, 0x15, struct d3dkmt_closeadapter)
#define LX_DXCHANGEVIDEOMEMORYRESERVATION \
	_IOWR(0x47, 0x16, struct d3dkmt_changevideomemoryreservation)
#define LX_DXCREATEHWCONTEXT		\
	_IOWR(0x47, 0x17, struct d3dkmt_createhwcontext)
#define LX_DXCREATEHWQUEUE		\
	_IOWR(0x47, 0x18, struct d3dkmt_createhwqueue)
#define LX_DXDESTROYDEVICE		\
	_IOWR(0x47, 0x19, struct d3dkmt_destroydevice)
#define LX_DXDESTROYHWCONTEXT		\
	_IOWR(0x47, 0x1a, struct d3dkmt_destroyhwcontext)
#define LX_DXDESTROYHWQUEUE		\
	_IOWR(0x47, 0x1b, struct d3dkmt_destroyhwqueue)
#define LX_DXDESTROYPAGINGQUEUE		\
	_IOWR(0x47, 0x1c, struct d3dddi_destroypagingqueue)
#define LX_DXDESTROYSYNCHRONIZATIONOBJECT \
	_IOWR(0x47, 0x1d, struct d3dkmt_destroysynchronizationobject)
#define LX_DXEVICT			\
	_IOWR(0x47, 0x1e, struct d3dkmt_evict)
#define LX_DXFLUSHHEAPTRANSITIONS	\
	_IOWR(0x47, 0x1f, struct d3dkmt_flushheaptransitions)
#define LX_DXFREEGPUVIRTUALADDRESS	\
	_IOWR(0x47, 0x20, struct d3dkmt_freegpuvirtualaddress)
#define LX_DXGETCONTEXTINPROCESSSCHEDULINGPRIORITY \
	_IOWR(0x47, 0x21, struct d3dkmt_getcontextinprocessschedulingpriority)
#define LX_DXGETCONTEXTSCHEDULINGPRIORITY \
	_IOWR(0x47, 0x22, struct d3dkmt_getcontextschedulingpriority)
#define LX_DXGETSHAREDRESOURCEADAPTERLUID \
	_IOWR(0x47, 0x23, struct d3dkmt_getsharedresourceadapterluid)
#define LX_DXINVALIDATECACHE		\
	_IOWR(0x47, 0x24, struct d3dkmt_invalidatecache)
#define LX_DXLOCK2			\
	_IOWR(0x47, 0x25, struct d3dkmt_lock2)
#define LX_DXMARKDEVICEASERROR		\
	_IOWR(0x47, 0x26, struct d3dkmt_markdeviceaserror)
#define LX_DXOFFERALLOCATIONS		\
	_IOWR(0x47, 0x27, struct d3dkmt_offerallocations)
#define LX_DXOPENRESOURCE		\
	_IOWR(0x47, 0x28, struct d3dkmt_openresource)
#define LX_DXOPENSYNCHRONIZATIONOBJECT	\
	_IOWR(0x47, 0x29, struct d3dkmt_opensynchronizationobject)
#define LX_DXQUERYALLOCATIONRESIDENCY	\
	_IOWR(0x47, 0x2a, struct d3dkmt_queryallocationresidency)
#define LX_DXQUERYRESOURCEINFO		\
	_IOWR(0x47, 0x2b, struct d3dkmt_queryresourceinfo)
#define LX_DXRECLAIMALLOCATIONS2	\
	_IOWR(0x47, 0x2c, struct d3dkmt_reclaimallocations2)
#define LX_DXRENDER			\
	_IOWR(0x47, 0x2d, struct d3dkmt_render)
#define LX_DXSETALLOCATIONPRIORITY	\
	_IOWR(0x47, 0x2e, struct d3dkmt_setallocationpriority)
#define LX_DXSETCONTEXTINPROCESSSCHEDULINGPRIORITY \
	_IOWR(0x47, 0x2f, struct d3dkmt_setcontextinprocessschedulingpriority)
#define LX_DXSETCONTEXTSCHEDULINGPRIORITY \
	_IOWR(0x47, 0x30, struct d3dkmt_setcontextschedulingpriority)
#define LX_DXSIGNALSYNCHRONIZATIONOBJECTFROMCPU \
	_IOWR(0x47, 0x31, struct d3dkmt_signalsynchronizationobjectfromcpu)
#define LX_DXSIGNALSYNCHRONIZATIONOBJECTFROMGPU \
	_IOWR(0x47, 0x32, struct d3dkmt_signalsynchronizationobjectfromgpu)
#define LX_DXSIGNALSYNCHRONIZATIONOBJECTFROMGPU2 \
	_IOWR(0x47, 0x33, struct d3dkmt_signalsynchronizationobjectfromgpu2)
#define LX_DXSUBMITCOMMANDTOHWQUEUE	\
	_IOWR(0x47, 0x34, struct d3dkmt_submitcommandtohwqueue)
#define LX_DXSUBMITSIGNALSYNCOBJECTSTOHWQUEUE \
	_IOWR(0x47, 0x35, struct d3dkmt_submitsignalsyncobjectstohwqueue)
#define LX_DXSUBMITWAITFORSYNCOBJECTSTOHWQUEUE \
	_IOWR(0x47, 0x36, struct d3dkmt_submitwaitforsyncobjectstohwqueue)
#define LX_DXUNLOCK2			\
	_IOWR(0x47, 0x37, struct d3dkmt_unlock2)
#define LX_DXUPDATEALLOCPROPERTY	\
	_IOWR(0x47, 0x38, struct d3dddi_updateallocproperty)
#define LX_DXUPDATEGPUVIRTUALADDRESS	\
	_IOWR(0x47, 0x39, struct d3dkmt_updategpuvirtualaddress)
#define LX_DXWAITFORSYNCHRONIZATIONOBJECTFROMCPU \
	_IOWR(0x47, 0x3a, struct d3dkmt_waitforsynchronizationobjectfromcpu)
#define LX_DXWAITFORSYNCHRONIZATIONOBJECTFROMGPU \
	_IOWR(0x47, 0x3b, struct d3dkmt_waitforsynchronizationobjectfromgpu)
#define LX_DXGETALLOCATIONPRIORITY	\
	_IOWR(0x47, 0x3c, struct d3dkmt_getallocationpriority)
#define LX_DXQUERYCLOCKCALIBRATION	\
	_IOWR(0x47, 0x3d, struct d3dkmt_queryclockcalibration)
#define LX_DXENUMADAPTERS3		\
	_IOWR(0x47, 0x3e, struct d3dkmt_enumadapters3)
#define LX_DXSHAREOBJECTS		\
	_IOWR(0x47, 0x3f, struct d3dkmt_shareobjects)
#define LX_DXOPENSYNCOBJECTFROMNTHANDLE2 \
	_IOWR(0x47, 0x40, struct d3dkmt_opensyncobjectfromnthandle2)
#define LX_DXQUERYRESOURCEINFOFROMNTHANDLE \
	_IOWR(0x47, 0x41, struct d3dkmt_queryresourceinfofromnthandle)
#define LX_DXOPENRESOURCEFROMNTHANDLE	\
	_IOWR(0x47, 0x42, struct d3dkmt_openresourcefromnthandle)
#define LX_DXQUERYSTATISTICS	\
	_IOWR(0x47, 0x43, struct d3dkmt_querystatistics)

#define LX_IO_MAX 0x43

#endif /* _D3DKMTHK_H */
