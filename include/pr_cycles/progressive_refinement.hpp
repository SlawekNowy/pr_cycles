/* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/.
*
* Copyright (c) 2023 Silverlan
*/

#ifndef __PR_CYCLES_PROGRESSIVE_REFINEMENT_HPP__
#define __PR_CYCLES_PROGRESSIVE_REFINEMENT_HPP__

#include <cinttypes>
#include <memory>
#include <sharedutils/functioncallback.h>
#include <util_raytracing/scene.hpp>
#include <util_raytracing/denoise.hpp>
#include <util_raytracing/tilemanager.hpp>
#include <util_raytracing/renderer.hpp>

namespace raytracing {
	class Scene;
};
namespace prosper {
	class Texture;
	class IImage;
	class IPrimaryCommandBuffer;
	class IFence;
};
namespace pragma::modules::cycles {
	class DenoiseTexture {
	  public:
		enum class DenoisingState : uint8_t { Initial = 0, Denoising, Complete };
		DenoiseTexture(uint32_t w, uint32_t h);
		~DenoiseTexture();

		void Denoise();
		bool IsDenoising() const;
		bool IsDenoisingComplete() const;
		void AppendTile(unirender::TileManager::TileData &&tileData);
		std::shared_ptr<uimg::ImageBuffer> GetDenoisedImageData() const;
	  private:
		void UpdatePendingTiles();
		void RunDenoise();

		std::mutex m_tileMutex;
		unirender::denoise::Denoiser m_denoiser;
		std::atomic<bool> m_running = false;
		std::atomic<bool> m_shouldDenoise = false;
		std::atomic<DenoisingState> m_denoisingState = DenoisingState::Initial;
		std::shared_ptr<uimg::ImageBuffer> m_inputImage = nullptr;
		std::shared_ptr<uimg::ImageBuffer> m_denoisedImage = nullptr;
		std::shared_ptr<uimg::ImageBuffer> m_outputImage = nullptr;
		std::queue<unirender::TileManager::TileData> m_pendingTiles;
		std::thread m_thread;
	};

	class ProgressiveTexture {
	  public:
		~ProgressiveTexture();
		void Initialize(unirender::Renderer &renderer);
		void Update();
		std::shared_ptr<prosper::Texture> GetTexture() const;
	  private:
		std::shared_ptr<prosper::IImage> CreateImage(uint32_t width, uint32_t height, bool onDevice) const;
		Vector2i m_tileSize;
		std::shared_ptr<prosper::IPrimaryCommandBuffer> m_cmdBuffer = nullptr;
		std::shared_ptr<prosper::IFence> m_fence = nullptr;
		std::shared_ptr<prosper::Texture> m_texture = nullptr;
		std::shared_ptr<prosper::IImage> m_image = nullptr;
		std::vector<std::shared_ptr<prosper::IImage>> m_mipImages;
		std::shared_ptr<unirender::Renderer> m_renderer = nullptr;
		CallbackHandle m_cbThink {};
	};
};

#endif
