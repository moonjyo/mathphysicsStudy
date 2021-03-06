
#include "Precompiled.h"
#include "SoftRenderer.h"

// 그리드 그리기
void SoftRenderer::DrawGrid2D()
{
	// 그리드 색상
	LinearColor gridColor(LinearColor(0.8f, 0.8f, 0.8f, 0.3f));

	// 가로 세로 라인 그리기
	ScreenPoint screenHalfSize = _ScreenSize.GetHalf();

	for (int x = screenHalfSize.X; x <= _ScreenSize.X; x += _Grid2DUnit)
	{
		_RSI->DrawFullVerticalLine(x, gridColor);
		if (x > screenHalfSize.X)
		{
			_RSI->DrawFullVerticalLine(2 * screenHalfSize.X - x, gridColor);
		}
	}

	for (int y = screenHalfSize.Y; y <= _ScreenSize.Y; y += _Grid2DUnit)
	{
		_RSI->DrawFullHorizontalLine(y, gridColor);
		if (y > screenHalfSize.Y)
		{
			_RSI->DrawFullHorizontalLine(2 * screenHalfSize.Y - y, gridColor);
		}
	}

	// 월드 축 그리기
	_RSI->DrawFullHorizontalLine(screenHalfSize.Y, LinearColor::Red);
	_RSI->DrawFullVerticalLine(screenHalfSize.X, LinearColor::Green);
}


// 게임 로직
void SoftRenderer::Update2D(float InDeltaSeconds)
{
	// 게임 로직에만 사용하는 변수
	static float shearSpeed = 2.f;
	static float scaleSpeed = 10.f;
	static float rotateSpeed = 180.f;

	// 엔진 모듈에서 입력 관리자 가져오기
	InputManager input = _GameEngine.GetInputManager();
	float deltaShear = input.GetXAxis() * shearSpeed * InDeltaSeconds; // X축은 밀기변환 수행 좌우방향키
	float deltaScale = input.GetZAxis() * scaleSpeed * InDeltaSeconds; // Z축 page up / down
	float deltaRotation = input.GetWAxis() * rotateSpeed * InDeltaSeconds; // W축 home/end

	_CurrentShear += deltaShear;
	_CurrentScale = Math::Clamp(_CurrentScale + deltaScale, 5.f, 15.f);
	_CurrentDegree += deltaRotation;
}

// 렌더링 로직
void SoftRenderer::Render2D()
{
	// 격자 그리기
	DrawGrid2D();

	static float increment = 0.001f;
	float rad = 0.f;
	static std::vector<Vector2> hearts;
	if (hearts.empty())
	{
		for (rad = 0.f; rad < Math::TwoPI; rad += increment)
		{
			/*float sin = sinf(rad);
			float cos = cosf(rad);
			float cos2 = cosf(2 * rad);
			float cos3 = cosf(3 * rad);
			float cos4 = cosf(4 * rad);
			float x = 16.f * sin * sin * sin;
			float y = 13 * cos - 5 * cos2 - 2 * cos3 - cos4;
			hearts.push_back(Vector2(x, y));*/

			float sin = sinf(rad);
			float sin2 = sinf(2 * rad);
			float sin3 = sinf(3 * rad);

			float cos = cosf(rad);
			float cos2 = cosf(2 * rad);
			float cos3 = cosf(3 * rad);

			float x = 5 * cos2 + 2 * cos3;
			float y = 2 * sin3 - 5 * sin2;
			hearts.push_back(Vector2(x, y));
		}
	}

	static const Vector2 pivot(200.f, 0.f);

	// 크기 행렬
	Matrix2x2 sMat (Vector2::UnitX * 1.f * _CurrentScale, Vector2::UnitY * 5.f *_CurrentScale);

	// 회전 행렬
	float sin, cos;
	Math::GetSinCos(sin, cos, _CurrentDegree);
	Matrix2x2 rMat (Vector2(cos, sin), Vector2(-sin, cos));

	// 밀기 행렬
	//Matrix2x2 shMat(Vector2::UnitX, Vector2(_CurrentShear, 1.f));
	Matrix2x2 shMat(Vector2(1.f, _CurrentShear), Vector2::UnitY);

	// 합성 행렬 ( 크기 -> 회전 -> 밀기)
	//Matrix2x2 finalMat = shMat * rMat * sMat;
	Matrix2x2 finalMat = shMat * sMat;

	// 각 값 초기화
	rad = 0.f;
	HSVColor hsv(0.f, 1.f, 0.85f); // 잘 보이도록 채도를 조금만 줄였음. 

	// 행렬 연산 횟수 
	int leftCnt = 0;
	int rightCnt = 0;
	for (auto const& v : hearts)
	{
		hsv.H = rad / Math::TwoPI;

		// 왼쪽 하트 ( 행렬을 각각 곱하기 )
		Vector2 left = sMat * v;
		left = rMat * left;
		left = shMat * left;
		leftCnt += 3;
		_RSI->DrawPoint(left - pivot, hsv.ToLinearColor());

		// 오른쪽 하트 ( 행렬을 한 번만 곱하기 )
		Vector2 right = finalMat * v;
		rightCnt++;
		_RSI->DrawPoint(right + pivot, hsv.ToLinearColor());

		rad += increment;
	}

	// 현재 위치와 스케일을 화면에 출력
	_RSI->PushStatisticText(std::string("Shear : ") + std::to_string(_CurrentShear));
	_RSI->PushStatisticText(std::string("Scale : ") + std::to_string(_CurrentScale));
	_RSI->PushStatisticText(std::string("Rotation : ") + std::to_string(_CurrentDegree));
	_RSI->PushStatisticText(std::string("Count (Left) : ") + std::to_string(leftCnt));
	_RSI->PushStatisticText(std::string("Count (Right) : ") + std::to_string(rightCnt));
}

