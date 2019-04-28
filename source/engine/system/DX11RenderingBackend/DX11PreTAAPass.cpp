#include "DX11PreTAAPass.h"
#include "DX11RenderingSystemUtilities.h"

#include "DX11LightPass.h"
#include "DX11SkyPass.h"

#include "../../component/DX11RenderingSystemComponent.h"

#include "../ICoreSystem.h"

extern ICoreSystem* g_pCoreSystem;

using namespace DX11RenderingSystemNS;

INNO_PRIVATE_SCOPE DX11PreTAAPass
{
	bool initializeShaders();

	DX11RenderPassComponent* m_DXRPC;

	DX11ShaderProgramComponent* m_DXSPC;

	ShaderFilePaths m_shaderFilePaths = { "DX11//preTAAPassVertex.hlsl" , "", "DX11//preTAAPassPixel.hlsl" };

	EntityID m_entityID;
}

bool DX11PreTAAPass::initialize()
{
	m_entityID = InnoMath::createEntityID();

	m_DXRPC = addDX11RenderPassComponent(1, DX11RenderingSystemComponent::get().deferredPassRTVDesc, DX11RenderingSystemComponent::get().deferredPassTextureDesc);

	initializeShaders();

	return true;
}

bool DX11PreTAAPass::initializeShaders()
{
	m_DXSPC = addDX11ShaderProgramComponent(m_entityID);

	m_DXSPC->m_samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	m_DXSPC->m_samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	m_DXSPC->m_samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	m_DXSPC->m_samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	m_DXSPC->m_samplerDesc.MipLODBias = 0.0f;
	m_DXSPC->m_samplerDesc.MaxAnisotropy = 1;
	m_DXSPC->m_samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	m_DXSPC->m_samplerDesc.BorderColor[0] = 0;
	m_DXSPC->m_samplerDesc.BorderColor[1] = 0;
	m_DXSPC->m_samplerDesc.BorderColor[2] = 0;
	m_DXSPC->m_samplerDesc.BorderColor[3] = 0;
	m_DXSPC->m_samplerDesc.MinLOD = 0;
	m_DXSPC->m_samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	initializeDX11ShaderProgramComponent(m_DXSPC, m_shaderFilePaths);

	return true;
}

bool DX11PreTAAPass::update()
{
	// Set the depth stencil state.
	DX11RenderingSystemComponent::get().m_deviceContext->OMSetDepthStencilState(
		m_DXRPC->m_depthStencilState, 1);

	// Set Rasterizer State
	DX11RenderingSystemComponent::get().m_deviceContext->RSSetState(
		DX11RenderingSystemComponent::get().m_rasterStateDeferred);

	activateDX11ShaderProgramComponent(m_DXSPC);

	activateRenderPass(m_DXRPC);

	// bind to previous pass render target textures
	bindTextureForRead(ShaderType::FRAGMENT, 0, DX11LightPass::getDX11RPC()->m_DXTDCs[0]);
	bindTextureForRead(ShaderType::FRAGMENT, 1, DX11SkyPass::getDX11RPC()->m_DXTDCs[0]);

	// draw
	auto l_MDC = getDX11MeshDataComponent(MeshShapeType::QUAD);
	drawMesh(l_MDC);

	unbindTextureForRead(ShaderType::FRAGMENT, 0);
	unbindTextureForRead(ShaderType::FRAGMENT, 1);

	return true;
}

bool DX11PreTAAPass::resize()
{
	return true;
}

bool DX11PreTAAPass::reloadShaders()
{
	return true;
}

DX11RenderPassComponent * DX11PreTAAPass::getDX11RPC()
{
	return m_DXRPC;
}