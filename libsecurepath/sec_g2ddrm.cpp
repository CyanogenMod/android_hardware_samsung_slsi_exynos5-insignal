/*
 * Copyright (C) 2012 Samsung Electronics Co., LTD
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdio.h>

#include "tlsecdrm_api.h"
#define LOG_TAG "sec_g2ddrm"
#include "log.h"
#include "tlc_communication.h"
#include "sec_g2ddrm.h"

mc_comm_ctx ctx;

static mcResult_t tlc_initialize(void) {
	mcResult_t mcRet;

	memset(&ctx, 0x00, sizeof(ctx));
	ctx.device_id = MC_DEVICE_ID_DEFAULT;
	ctx.uuid = (mcUuid_t)TL_SECDRM_UUID;
	ctx.initialized = false;

	mcRet = tlc_open(&ctx);
	if (MC_DRV_OK != mcRet) {
		   LOG_E("open TL session failed!");
		   return mcRet;
	}

	ctx.initialized = true;

	return MC_DRV_OK;
}

static mcResult_t tlc_terminate(void) {
	mcResult_t mcRet;

	if (ctx.initialized == true) {
		mcRet = tlc_close(&ctx);
		if (MC_DRV_OK != mcRet) {
			   LOG_E("close TL session failed!");
			   return mcRet;
		}

		memset(&ctx, 0x00, sizeof(ctx));
		ctx.initialized = false;
	}

	return MC_DRV_OK;
}

extern "C" g2ddrmResult_t G2DDRM_Initialize(void)
{
	mcResult_t mcRet;
	g2ddrmResult_t ret = G2DDRM_SUCCESS;
	struct tciMessage_t *tci = NULL;

	LOG_I("G2DDRM_Initialize(): secure G2D driver initialization");
	do {
		LOG_I("Open the Trustlet");

		mcRet = tlc_initialize();
		if (MC_DRV_OK != mcRet) {
			LOG_E("Tlc Open Error");
			ret = G2DDRM_ERROR_INIT_FAILED;
			break;
		}

		LOG_I("Check TCI buffer");
		tci = ctx.tci_msg;
		if (NULL == tci) {
			LOG_E("TCI has not been set up properly - exiting");
			ret = G2DDRM_ERROR_INIT_FAILED;
			break;
		}

		LOG_I("Prepare command message in TCI");
		tci->cmd.id = CMD_G2DDRM_INITIALIZE;
		mcRet = tlc_communicate(&ctx);
		if (MC_DRV_OK != mcRet) {
			LOG_E("tlc_communicate Error!");
			ret = G2DDRM_ERROR_INIT_FAILED;
			break;
		}

		if ((RSP_ID(CMD_G2DDRM_INITIALIZE) != tci->resp.id)) {
			LOG_E("Trustlet did not send a response : %d", tci->resp.id);
			ret = G2DDRM_ERROR_INIT_FAILED;
			break;
		}
		LOG_I("Trustlet response is completed");

		if (tci->resp.return_code != RET_TL_G2DDRM_OK) {
			LOG_E("Trustlet did not send a valid return code : %d", tci->resp.return_code);
			ret = G2DDRM_ERROR_INIT_FAILED;
			break;
		}
		LOG_I("Check the Trustlet return code is completed");

		ret = G2DDRM_SUCCESS;
	} while (false);

	LOG_I("G2DDRM_Initialize(): secure G2D driver is initialized. ret(%d)", ret);
	return ret;
}

extern "C" g2ddrmResult_t G2DDRM_Blit(struct fimg2d_blit_raw *cmd)
{
	mcResult_t mcRet;
	g2ddrmResult_t ret = G2DDRM_SUCCESS;
	mcBulkMap_t mapInfo;
	struct tciMessage_t *tci = NULL;

	mapInfo.sVirtualAddr = NULL;

	do {
		tci->cmd.id = CMD_G2DDRM_BLIT;
		tci->blit.op = cmd->op;
		tci->blit.param = cmd->param;
		tci->blit.src = cmd->src;
		tci->blit.dst = cmd->dst;
		tci->blit.msk = cmd->msk;
		tci->blit.tmp = cmd->tmp;
		tci->blit.sync = cmd->sync;
		tci->blit.seq_no = cmd->seq_no;

		mcRet = tlc_communicate(&ctx);
		if (MC_DRV_OK != mcRet) {
			LOG_E("tlc_communicate Error!");
			ret = G2DDRM_ERROR_BLIT_FAILED;
			break;
		}

		if ((RSP_ID(CMD_G2DDRM_BLIT) != tci->resp.id)) {
			LOG_E("Trustlet did not send a response : %d", tci->resp.id);
			ret = G2DDRM_ERROR_BLIT_FAILED;
			break;
		}
		LOG_I("Trustlet response is completed");

		if (tci->resp.return_code != RET_TL_G2DDRM_OK) {
			LOG_E("Trustlet did not send a valid return code : %d", tci->resp.return_code);
			ret = G2DDRM_ERROR_BLIT_FAILED;
			break;
		}
		LOG_I("Check the Trustlet return code is completed");

		ret = G2DDRM_SUCCESS;
	} while (false);

	LOG_I("G2DDRM_Blit(): secure G2D driver blit is done. ret(%d)", ret);
	return ret;
}

extern "C" g2ddrmResult_t G2DDRM_Terminate(void)
{
	mcResult_t mcRet;
	g2ddrmResult_t ret = G2DDRM_SUCCESS;
	struct tciMessage_t *tci = NULL;

	LOG_I("G2DDRM_Terminate(): secure G2D driver termination");
	do {
		LOG_I("Prepare command message in TCI");
		tci->cmd.id = CMD_G2DDRM_TERMINATE;

		mcRet = tlc_communicate(&ctx);
		if (MC_DRV_OK != mcRet) {
			LOG_E("tlc_communicate Error!");
			ret = G2DDRM_ERROR_EXIT_FAILED;
			break;
		}

		if ((RSP_ID(CMD_G2DDRM_TERMINATE) != tci->resp.id)) {
			LOG_E("Trustlet did not send a response : %d", tci->resp.id);
			ret = G2DDRM_ERROR_EXIT_FAILED;
			break;
		}
		LOG_I("Trustlet response is completed");

		if (tci->resp.return_code != RET_TL_G2DDRM_OK) {
			LOG_E("Trustlet did not send a valid return code : %d", tci->resp.return_code);
			ret = G2DDRM_ERROR_EXIT_FAILED;
			break;
		}

		mcRet = tlc_terminate();
		if (MC_DRV_OK != mcRet) {
			LOG_E("Tlc Close Error");
			ret = G2DDRM_ERROR_INIT_FAILED;
			break;
		}

		LOG_I("Check the Trustlet return code is completed");

		ret = G2DDRM_SUCCESS;
	} while (false);

	LOG_I("G2DDRM_Terminate(): secure G2D driver is terminated. ret(%d)", ret);
	return ret;
}
