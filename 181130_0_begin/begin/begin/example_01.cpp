#include "kGlobalDefine.h"
#include "example_01.h"

#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")

void example_01::init()
{
	std::function<void*(NUM_REAL, NUM_REAL, NUM_REAL)> actPrintPos = [](NUM_REAL x, NUM_REAL y, NUM_REAL z)->void* { printf("��ġ : %.1f, %.1f, %.1f\n", x, y, z); return 0; };
	std::function<void*(NUM_REAL, NUM_REAL, NUM_REAL)> actPrintDir = [](NUM_REAL x, NUM_REAL y, NUM_REAL z)->void* { printf("���� : %.1f, %.1f, %.1f\n", x, y, z); return 0; };

	c3DVector vPos(1.5, 1.2, 1);
	c3DVector vDir(1, 1, 0);
	
	printf("< ���� ���� >\n");
	c3DVector::actXYZ(vPos, actPrintPos);
	c3DVector::actXYZ(vDir, actPrintPos);

	c3DMatrix m;
	c3DVector vChangePos;
	c3DVector vChangeDir;

	printf("\n< ��ġ ��ȯ >\n");
	double inputTranslate[] = { 2.0, 3.0 };
	vChangePos = vPos;
	vChangeDir = vDir;
	m = c3DMatrix::getTranslate(inputTranslate);

	vChangePos = vChangePos.getTransform(m);
	vChangeDir = vChangeDir.getTransform(m);
	c3DVector::actXYZ(vChangePos, actPrintPos);
	c3DVector::actXYZ(vChangeDir, actPrintPos);

	printf("\n< ȸ�� ��ȯ >\n");
	double inputRotate = D3DX_PI / 3.0;
	vChangePos = vPos;
	vChangeDir = vDir;
	m = c3DMatrix::getRotate(inputRotate);

	vChangePos = vChangePos.getTransform(m);
	vChangeDir = vChangeDir.getTransform(m);
	c3DVector::actXYZ(vChangePos, actPrintPos);
	c3DVector::actXYZ(vChangeDir, actPrintPos);

	printf("\n< ô�� ��ȯ >\n");
	double inputScale[] = { 8.0, 2.0 };
	vChangePos = vPos;
	vChangeDir = vDir;
	m = c3DMatrix::getScale(inputScale);

	vChangePos = vChangePos.getTransform(m);
	vChangeDir = vChangeDir.getTransform(m);
	c3DVector::actXYZ(vChangePos, actPrintPos);
	c3DVector::actXYZ(vChangeDir, actPrintPos);

	printf("\n\n");
	system("pause");

}