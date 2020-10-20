
#include "Precompiled.h"

WindowsRSI::~WindowsRSI()
{
}

bool WindowsRSI::Init(const ScreenPoint& InScreenSize)
{
	return InitializeGDI(InScreenSize);
}

void WindowsRSI::Shutdown()
{
	ReleaseGDI();
}

void WindowsRSI::Clear(const LinearColor & InClearColor)
{
	FillBuffer(InClearColor.ToColor32());
	ClearDepthBuffer();
}

void WindowsRSI::BeginFrame()
{
}

void WindowsRSI::EndFrame()
{
	SwapBuffer();
}

void WindowsRSI::DrawFullVerticalLine(int InX, const LinearColor & InColor)
{
	if (InX < 0 || InX >= _ScreenSize.X)
	{
		return;
	}

	for (int y = 0; y < _ScreenSize.Y; ++y)
	{
		SetPixel(ScreenPoint(InX, y), InColor);
	}
}

void WindowsRSI::DrawFullHorizontalLine(int InY, const LinearColor & InColor)
{
	if (InY < 0 || InY >= _ScreenSize.Y)
	{
		return;
	}

	for (int x = 0; x < _ScreenSize.X; ++x)
	{
		SetPixel(ScreenPoint(x, InY), InColor);
	}
}

void WindowsRSI::DrawPoint(const Vector2& InVectorPos, const LinearColor& InColor)
{
	SetPixel(ScreenPoint::ToScreenCoordinate(_ScreenSize, InVectorPos), InColor);
}

void WindowsRSI::DrawPoint(const ScreenPoint& InScreenPos, const LinearColor& InColor)
{
	SetPixel(InScreenPos, InColor);
}

int WindowsRSI::TestRegion(const Vector2& InVectorPos, const Vector2& InMinPos, const Vector2& InMaxPos)
{
	int result = 0;
	if (InVectorPos.X < InMinPos.X)
	{
		result = result | 0b0001;
	}
	else if (InVectorPos.X > InMaxPos.X)
	{
		result = result | 0b0010;
	}

	if (InVectorPos.Y < InMinPos.Y)
	{
		result = result | 0b0100;
	}
	else if (InVectorPos.Y > InMaxPos.Y)
	{
		result = result | 0b1000;
	}

	return result;
}

bool WindowsRSI::CohenSutherlandLineClip(Vector2& InOutStartPos, Vector2& InOutEndPos, const Vector2& InMinPos, const Vector2& InMaxPos)
{
	int startTest = TestRegion(InOutStartPos, InMinPos, InMaxPos);
	int endTest = TestRegion(InOutEndPos, InMinPos, InMaxPos);

	float width = (InOutEndPos.X - InOutStartPos.X);
	float height = (InOutEndPos.Y - InOutStartPos.Y);

	while (true)
	{
		if ((startTest == 0) && (endTest == 0))
		{
			return true;
		}
		else if (startTest & endTest)
		{
			return false;
		}
		else
		{
			Vector2 clippedPosition;
			bool isStartTest = (startTest != 0);
			int currentTest = isStartTest ? startTest : endTest;

			if (currentTest < 0b0100)
			{
				if (currentTest & 1)
				{
					clippedPosition.X = InMinPos.X;
				}
				else
				{
					clippedPosition.X = InMaxPos.X;
				}

				if (Math::EqualsInTolerance(height, 0.0f))
				{
					clippedPosition.Y = InOutStartPos.Y;

				}
				else
				{
					clippedPosition.Y = InOutStartPos.Y + height * (clippedPosition.X - InOutStartPos.X) / width;
				}
			}
			else
			{
				if (currentTest & 0b0100)
				{
					clippedPosition.Y = InMinPos.Y;
				}
				else
				{
					clippedPosition.Y = InMaxPos.Y;
				}

				if (Math::EqualsInTolerance(width, 0.0f))
				{
					clippedPosition.X = InOutStartPos.X;
				}
				else
				{
					clippedPosition.X = InOutStartPos.X + width * (clippedPosition.Y - InOutStartPos.Y) / height;
				}
			}

			if (isStartTest)
			{
				InOutStartPos = clippedPosition;
				startTest = TestRegion(InOutStartPos, InMinPos, InMaxPos);
			}
			else
			{
				InOutEndPos = clippedPosition;
				endTest = TestRegion(InOutEndPos, InMinPos, InMaxPos);
			}
		}
	}

	return true;
}

void WindowsRSI::DrawLine(const Vector4& InStartPos, const Vector4& InEndPos, const LinearColor& InColor)
{
	DrawLine(InStartPos.ToVector2(), InEndPos.ToVector2(), InColor);
}


float WindowsRSI::GetDepthBufferValue(const ScreenPoint& InPos) const
{
	if (_DepthBuffer == nullptr)
	{
		return INFINITY;
	}

	if (!IsInScreen(InPos))
	{
		return INFINITY;
	}

	return *(_DepthBuffer + GetScreenBufferIndex(InPos));
}

void WindowsRSI::SetDepthBufferValue(const ScreenPoint& InPos, float InDepthValue)
{
	if (_DepthBuffer == nullptr)
	{
		return;
	}

	if (!IsInScreen(InPos))
	{
		return;
	}

	*(_DepthBuffer + GetScreenBufferIndex(InPos)) = InDepthValue;
}

void WindowsRSI::DrawLine(const Vector2& InStartPos, const Vector2& InEndPos, const LinearColor& InColor)
{
	Vector2 clippedStart = InStartPos;
	Vector2 clippedEnd = InEndPos;
	Vector2 screenExtend = Vector2(_ScreenSize.X, _ScreenSize.Y) * 0.5f;
	Vector2 minScreen = -screenExtend;
	Vector2 maxScreen = screenExtend;
	if (!CohenSutherlandLineClip(clippedStart, clippedEnd, minScreen, maxScreen))
	{
		return;
	}

	ScreenPoint startPosition = ScreenPoint::ToScreenCoordinate(_ScreenSize, clippedStart);
	ScreenPoint endPosition = ScreenPoint::ToScreenCoordinate(_ScreenSize, clippedEnd);

	int width = endPosition.X - startPosition.X;
	int height = endPosition.Y - startPosition.Y;

	// 여기에 관련 코드를 구현하기.
	
	int ah = 2;
	int bw = 1;	

	for (int i = startPosition.X; i <= endPosition.X; i++)
	{
		// 8분면의 판별식 2h+w < 0
		if (ah * height + bw * width > 0) // 판별식 크면 직진
		{	
			startPosition.X += 1;
			ah += 2;

		}
		else if (ah * height + bw * width < 0) // 중점보다 작으면 위쪽을 찍음
		{
			startPosition.X += 1;
			startPosition.Y -= 1;
			ah += 2;
			bw += 2;

		}
		SetPixel(startPosition, InColor);
	}

}

void WindowsRSI::PushStatisticText(std::string && InText)
{
	_StatisticTexts.emplace_back(InText);
}

void WindowsRSI::PushStatisticTexts(std::vector<std::string> && InTexts)
{
	std::move(InTexts.begin(), InTexts.end(), std::back_inserter(_StatisticTexts));
}
