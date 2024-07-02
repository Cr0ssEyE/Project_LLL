// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/RichText/LLL_RichTextToolTipDecorator.h"

/*class FLLL_RichTextToolTip : public FRichTextDecorator
{
public:
	FLLL_RichTextToolTip(URichTextBlock* InOwner, const FTextBlockStyle& HoverTextStyle, const FTextBlockStyle& TooltipTextStyle) :
		FRichTextDecorator(InOwner),
		HoverTextBlockStyle(HoverTextStyle),
		ToolTipTextBlockStyle(TooltipTextStyle)
	{
		
	}

	virtual bool Supports(const FTextRunParseResults& RunInfo, const FString& Text) const override
	{
		return RunInfo.Name == TEXT("tooltip") && RunInfo.MetaData.Contains(TEXT("text"));
	}

protected:
	virtual TSharedPtr<SWidget> CreateDecoratorWidget(const FTextRunInfo& RunInfo, const FTextBlockStyle& DefaultTextStyle) const override
	{
		return SNew(STextBlock)
			.Text(RunInfo.Content)
			.TextStyle(&HoverTextBlockStyle)
			.ToolTip(SNew(SToolTip)
			[
				SNew(STextBlock)
				.Text(FText::FromString(RunInfo.MetaData[TEXT("text")]))
				.TextStyle(&ToolTipTextBlockStyle)
			]);
	}
	
private:
	FTextBlockStyle HoverTextBlockStyle;
	FTextBlockStyle ToolTipTextBlockStyle;
	
};*/

ULLL_RichTextToolTipDecorator::ULLL_RichTextToolTipDecorator(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	
}

/*TSharedPtr<ITextDecorator> ULLL_RichTextToolTipDecorator::CreateDecorator(URichTextBlock* InOwner)
{
	return MakeShareable(new FLLL_RichTextToolTip(InOwner, TextBlockStyle, TooltipTextBlockStyle));
}*/
