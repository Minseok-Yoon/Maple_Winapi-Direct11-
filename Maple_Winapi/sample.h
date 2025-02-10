#pragma once

// D3D11_TEXTURE2D_DESC ����ü �ʵ� ����
//typedef struct D3D11_TEXTURE2D_DESC
//{
//    UINT Width;                       // �ؽ�ó�� �ʺ� (�ȼ� ����)
//    UINT Height;                      // �ؽ�ó�� ���� (�ȼ� ����)
//    UINT MipLevels;                   // Mipmap ������ ��
//    UINT ArraySize;                   // �ؽ�ó �迭�� ũ�� (���� 1, Cube �ؽ�ó�� 6)
//    DXGI_FORMAT Format;               // �ؽ�ó ������ ���� (��: DXGI_FORMAT_R8G8B8A8_UNORM)
//    DXGI_SAMPLE_DESC SampleDesc;      // ��Ƽ���ø� (Anti-Aliasing) ����
//    D3D11_USAGE Usage;                // �ؽ�ó�� ��� ��� (��: D3D11_USAGE_DEFAULT)
//    UINT BindFlags;                   // �ؽ�ó�� ���� Ÿ��, ���̴� ���ҽ� ������ ������� ����
//    UINT CPUAccessFlags;              // CPU���� ���� �������� ���� (��: D3D11_CPU_ACCESS_WRITE)
//    UINT MiscFlags;                   // ��Ÿ ���� �÷��� (��: D3D11_RESOURCE_MISC_GENERATE_MIPS)
//} D3D11_TEXTURE2D_DESC;

// Miplevel - �ؽ�ó�� ���� ��ҵ� ������ ���� �ؽ�ó ���͸� ������ ���̴µ� ���ȴ�.
// 0���� �����ϸ� DirectX�� �ִ� Mipmap ������ �ڵ����� �����ϰ� 1�� �����ϸ� �⺻ �ؽ�ó�� ����Ѵ�.

// ArraySize - �ؽ�ó �迭�� ũ�⸦ �����Ѵ�. �⺻������ 1�� �����ȴ�.
// 1���� ũ�� ������ �ؽ�ó ������ ������ ���� �ؽ�ó�� �迭 ���·� �����ȴ�.(ex. CubeMap �ؽ�ó�� ��� 6)

// Format - �ؽ�ó�� �ȼ� ������ ������ �����Ѵ�. �� ������ �ؽ�ó�� �� �ȼ��� ��� �������� �����Ѵ�.
// DXGI_FORMAT_R8G8B8A8_UNORM : 8��Ʈ�� 4ä��(R,G,B,A)�� ����ȭ�� ��(0~1)���� ����
// DXGI_FORMAT_D24_UNORM_S8_UINT: 24��Ʈ ���� ���� 8��Ʈ ���ٽ� ������ ����
// DXGI_FORMAT_R32_FLOAT : 32��Ʈ �Ҽ� ������ ����

// SampleDesc - �ؽ�ó�� ������ ��Ƽ���ø�(MSAA) ������ �����Ѵ�. ��Ƽ ���ø��� �ٸ����(�������)�� ���̴µ� ���ȴ�.
// Count: ��Ƽ���ø��� ���� ���� �����Ѵ�. 1�� ��Ƽ ���ø��� ������� ������ �ǹ�
// Quality: �� ������ ǰ���� �����Ѵ�.

// Usage - �ؽ�ó�� ��� ����� �����Ѵ�. �� ���� ���� �ؽ�ó�� GPU�� ��� �������� �����ȴ�.
//�ַ� ���Ǵ� ��
//D3D11_USAGE_DEFAULT : GPU���� �а� ����, CPU ������ �Ұ�
//D3D11_USAGE_DYNAMIC : CPU���� �ֱ������� �����͸� ������Ʈ �� �� ����
//D3D11_USAGE_IMMUTABLE : �ʱ�ȭ �� ������ ���� �Ұ�, ���� �б� ���� ���ҽ��� ���
//�� : ���� Ÿ���̳� ���� ���ۿ����� ����Ϸ��� D3D11_USAGE_DEFAULT�� �����Ѵ�

// BindFlags - �ؽ�ó�� � �뵵�� ���ε� ������ ����. 
//�ַ� ���Ǵ� �÷��� :
//D3D11_BIND_RENDER_TARGET: ���� Ÿ������ ���ε��� �� ����
//D3D11_BIND_DEPTH_STENCIL : ���� ���ٽ� ���۷� ����� �� ����
//D3D11_BIND_SHADER_RESOURCE : ���̴����� ���ҽ��� ��� �� �� ����
//�� : D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE�� �ؽ�ó�� ����Ÿ���̸鼭 ���̴����� ���ҽ��ε� ����� �� ������ �ǹ��Ѵ�.

// CPUAccessFlags - �ؽ�ó�� CPU���� ������ �� �ִ��� ���� ����
//�ַ� ���Ǵ� �� :
//D3D11_CPU_ACCESS_WRITE: CPU���� ���� ����
//D3D11_CPU_ACCESS_READ : CPU���� �б� ����
//�⺻���� 0���� �����Ǿ� CPU������ ������� �ʴ´�
//�� : CPU�� �ؽ�ó �����͸� �����ؾ� �ϴ� ��� D3D11_CPU_ACCESS_WRITE�� �����Ѵ�.

// MiscFlags - �ؽ�ó�� ���� ��Ÿ ������ �����Ѵ�. �߰����� �ؽ�ó Ư���̳� ������ ������ �� �ִ�.
//�ַ� ���Ǵ� �� :
//D3D11_RESOURCE_MISC_GENERATE_MIPS: Mipmap�� ������ �� �ֵ��� ����.
//D3D11_RESOURCE_MISC_TEXTURECUBE : �ؽ�ó�� ť�� ������ ���
//�� : �ڵ����� Mipmap�� �����ϰ� �ʹٸ� D3D11_RESOURCE_MISC_GENERATE_MIPS�� ����Ѵ�.