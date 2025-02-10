#pragma once

// D3D11_TEXTURE2D_DESC 구조체 필드 설명
//typedef struct D3D11_TEXTURE2D_DESC
//{
//    UINT Width;                       // 텍스처의 너비 (픽셀 단위)
//    UINT Height;                      // 텍스처의 높이 (픽셀 단위)
//    UINT MipLevels;                   // Mipmap 레벨의 수
//    UINT ArraySize;                   // 텍스처 배열의 크기 (보통 1, Cube 텍스처는 6)
//    DXGI_FORMAT Format;               // 텍스처 데이터 포맷 (예: DXGI_FORMAT_R8G8B8A8_UNORM)
//    DXGI_SAMPLE_DESC SampleDesc;      // 멀티샘플링 (Anti-Aliasing) 정보
//    D3D11_USAGE Usage;                // 텍스처의 사용 방법 (예: D3D11_USAGE_DEFAULT)
//    UINT BindFlags;                   // 텍스처를 렌더 타겟, 셰이더 리소스 등으로 사용할지 지정
//    UINT CPUAccessFlags;              // CPU에서 접근 가능한지 여부 (예: D3D11_CPU_ACCESS_WRITE)
//    UINT MiscFlags;                   // 기타 설정 플래그 (예: D3D11_RESOURCE_MISC_GENERATE_MIPS)
//} D3D11_TEXTURE2D_DESC;

// Miplevel - 텍스처의 여러 축소된 버전을 만들어서 텍스처 필터링 성능을 높이는데 사용된다.
// 0으로 설정하면 DirectX가 최대 Mipmap 레벨을 자동으로 생성하고 1로 설정하면 기본 텍스처만 사용한다.

// ArraySize - 텍스처 배열의 크기를 정의한다. 기본적으로 1로 설정된다.
// 1보다 크면 동일한 텍스처 포맷을 가지는 여러 텍스처가 배열 형태로 관리된다.(ex. CubeMap 텍스처의 경우 6)

// Format - 텍스처의 픽셀 데이터 포맷을 정의한다. 이 포맷은 텍스처의 각 픽셀을 어떻게 저장할지 결정한다.
// DXGI_FORMAT_R8G8B8A8_UNORM : 8비트씩 4채널(R,G,B,A)를 정규화된 값(0~1)으로 저장
// DXGI_FORMAT_D24_UNORM_S8_UINT: 24비트 깊이 값과 8비트 스텐실 값으로 저장
// DXGI_FORMAT_R32_FLOAT : 32비트 소수 값으로 저장

// SampleDesc - 텍스처에 적용할 멀티샘플링(MSAA) 정보를 정의한다. 멀티 샘플링은 앨리어싱(계단현상)을 줄이는데 사용된다.
// Count: 멀티샘플링의 샘플 수를 정의한다. 1은 멀티 샘플링을 사용하지 않음을 의미
// Quality: 각 샘플의 품질을 정의한다.

// Usage - 텍스처의 사용 방법을 정의한다. 이 값에 따라 텍스처가 GPU에 어떻게 사용될지가 결정된다.
//주로 사용되는 값
//D3D11_USAGE_DEFAULT : GPU에서 읽고 쓰며, CPU 접근은 불가
//D3D11_USAGE_DYNAMIC : CPU에서 주기적으로 데이터를 업데이트 할 수 있음
//D3D11_USAGE_IMMUTABLE : 초기화 후 데이터 변경 불가, 빠른 읽기 전용 리소스로 사용
//예 : 렌더 타겟이나 깊이 버퍼용으로 사용하려면 D3D11_USAGE_DEFAULT로 설정한다

// BindFlags - 텍스처가 어떤 용도로 바인딩 될지를 정의. 
//주로 사용되는 플래스 :
//D3D11_BIND_RENDER_TARGET: 렌더 타겟으로 바인딩할 수 있음
//D3D11_BIND_DEPTH_STENCIL : 깊이 스텐실 버퍼로 사용할 수 있음
//D3D11_BIND_SHADER_RESOURCE : 셰이더에서 리소스로 사용 할 수 있음
//예 : D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE는 텍스처를 렌더타겟이면서 셰이더에서 리소스로도 사용할 수 있음을 의미한다.

// CPUAccessFlags - 텍스처에 CPU에서 접근할 수 있는지 여부 정의
//주로 사용되는 값 :
//D3D11_CPU_ACCESS_WRITE: CPU에서 쓰기 가능
//D3D11_CPU_ACCESS_READ : CPU에서 읽기 가능
//기본값은 0으로 설정되어 CPU접근을 허용하지 않는다
//예 : CPU가 텍스처 데이터를 수정해야 하는 경우 D3D11_CPU_ACCESS_WRITE를 설정한다.

// MiscFlags - 텍스처에 대한 기타 설정을 정의한다. 추가적인 텍스처 특성이나 동작을 지정할 수 있다.
//주로 사용되는 값 :
//D3D11_RESOURCE_MISC_GENERATE_MIPS: Mipmap을 생성할 수 있도록 설정.
//D3D11_RESOURCE_MISC_TEXTURECUBE : 텍스처를 큐브 맵으로 사용
//예 : 자동으로 Mipmap을 생성하고 싶다면 D3D11_RESOURCE_MISC_GENERATE_MIPS를 사용한다.