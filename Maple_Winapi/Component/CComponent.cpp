#include "CComponent.h"

CComponent::CComponent(COMPONENT_TYPE _eComponentType) :
	m_eComponentType(_eComponentType),
	m_pOwner(nullptr)
{
}

CComponent::~CComponent()
{
}

void CComponent::Init()
{
}

void CComponent::Update()
{
}

void CComponent::LateUpdate()
{
}

void CComponent::Render()
{
}