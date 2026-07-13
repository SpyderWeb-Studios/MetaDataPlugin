#pragma once


#include "DetailCategoryBuilder.h"
#include "DetailLayoutBuilder.h"
#include "DetailWidgetRow.h"
#include "IDetailCustomization.h"
#include "Subsystems/MetaDataEditorSubsystem.h"
#include "Subsystems/MetaDataIndexerSubsystem.h"

class FMetaDataSettingsCustomisation : public IDetailCustomization {
public:
	static TSharedRef<IDetailCustomization> MakeInstance();
	virtual void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override;
};

inline TSharedRef<IDetailCustomization> FMetaDataSettingsCustomisation::MakeInstance()
{
	return MakeShared<FMetaDataSettingsCustomisation>();
}

inline void FMetaDataSettingsCustomisation::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
    
    DetailBuilder.EditCategory("Routing");
    DetailBuilder.EditCategory("Naming");
    
	TSharedPtr<IPropertyHandle> MapProperty = DetailBuilder.GetProperty("CachedAssets");
    IDetailCategoryBuilder& Category = DetailBuilder.EditCategory("Cache");
    
    // Add in an action button row
    Category.AddCustomRow(FText::FromString("Cache Actions"))
[
    SNew(SHorizontalBox)
    + SHorizontalBox::Slot()
    .AutoWidth()
    .Padding(0, 0, 10, 0)
    .VAlign(VAlign_Center)
    [
        SNew(SButton)
        .Text(FText::FromString("Refresh All"))
        .OnClicked_Lambda([]() { /* Logic */ return FReply::Handled(); })
    ]
    + SHorizontalBox::Slot()
    .AutoWidth()
    .Padding(0, 0, 10, 0)
    .VAlign(VAlign_Center)
    [
        SNew(SButton)
        .Text(FText::FromString("Clear Cache"))
        .OnClicked_Lambda([]() { /* Logic */ return FReply::Handled(); })
    ]
    + SHorizontalBox::Slot()
    .AutoWidth()
    .Padding(0, 0, 10, 0)
    .VAlign(VAlign_Center)
    [
        SNew(SButton)
        .Text(FText::FromString("Extract From Cache"))
        .OnClicked_Lambda([]() { /* Logic */ return FReply::Handled(); })
    ]
    ];

    // Hide the default map view
    Category.AddProperty(MapProperty).Visibility(EVisibility::Collapsed);
  
    // Create a custom UI for the map items
    TSharedPtr<IPropertyHandleSet> MapHandle = MapProperty->AsSet();
    uint32 NumChildren = 0;
    MapProperty->GetNumChildren(NumChildren);

    UMetaDataIndexerSubsystem* MetaDataIndexerSubsystem = GEditor->GetEditorSubsystem<UMetaDataIndexerSubsystem>();
    UMetaDataEditorSubsystem* MetaDataCoreEditorSubsystem = GEditor->GetEditorSubsystem<UMetaDataEditorSubsystem>();
    check(MetaDataIndexerSubsystem);
    
    for (uint32 i = 0; i < NumChildren; ++i)
    {
       TSharedPtr<IPropertyHandle> ElementHandle = MapHandle->GetElement(i);

        Category.AddCustomRow(FText::FromString("AssetRow"))
.NameContent()
[
    SNew(SHorizontalBox)
    // Asset
    + SHorizontalBox::Slot()
    .FillWidth(1.0f)
    .VAlign(VAlign_Center)
    [
        ElementHandle->CreatePropertyValueWidget()
    ]
]
.ValueContent()
[
    SNew(SHorizontalBox)
    // Status Badge column (Fixed width for alignment)
    + SHorizontalBox::Slot()
    .AutoWidth()
    .Padding(8, 0)
    .VAlign(VAlign_Center)
    [
        SNew(SBox)
        .WidthOverride(20.0f)
        .HeightOverride(20.0f)
        [
            SNew(SImage)
            
            .Image_Lambda([MetaDataIndexerSubsystem, ElementHandle]() -> const FSlateBrush*
            {
                EMetaDataBakingAssetStatus CurrentStatus = EMetaDataBakingAssetStatus::MDBAS_NONE;
                
                FString AssetPathString;
                if (ElementHandle->GetValueAsFormattedString(AssetPathString) == FPropertyAccess::Success)
                {
                    FSoftObjectPath SoftPath(AssetPathString);
                    CurrentStatus = MetaDataIndexerSubsystem->GetSoftAssetStatus(SoftPath);
                }
                    
                    switch (CurrentStatus) {
                        case EMetaDataBakingAssetStatus::MDBAS_Baked:
                            return FAppStyle::GetBrush("Icons.SuccessWithColor");
                    case EMetaDataBakingAssetStatus::MDBAS_SavedOnly: 
                        return FAppStyle::GetBrush("Icons.Success");
                    case EMetaDataBakingAssetStatus::MDBAS_Indexed: 
                        return FAppStyle::GetBrush("Icons.Warning");
                    case EMetaDataBakingAssetStatus::MDBAS_Empty:
                        return FAppStyle::GetBrush("Icons.Denied");
                        case EMetaDataBakingAssetStatus::MDBAS_NONE: break;
                        default: break;
                    }
                
                return nullptr;
            })
            .ToolTipText_Lambda([MetaDataIndexerSubsystem, ElementHandle]() -> FText
            {
                EMetaDataBakingAssetStatus CurrentStatus = EMetaDataBakingAssetStatus::MDBAS_NONE;
                
                 FString AssetPathString;
                 if (ElementHandle->GetValueAsFormattedString(AssetPathString) == FPropertyAccess::Success)
                 {
                     FSoftObjectPath SoftPath(AssetPathString);
                     CurrentStatus = MetaDataIndexerSubsystem->GetSoftAssetStatus(SoftPath);
                 }
                
	                switch (CurrentStatus)
					{
					case EMetaDataBakingAssetStatus::MDBAS_Baked:
						{
							return FText::FromString("Meta Data Baked");
						}
					case EMetaDataBakingAssetStatus::MDBAS_SavedOnly:
						{
							return FText::FromString("Asset Saved, Meta Data not baked");
						}
					case EMetaDataBakingAssetStatus::MDBAS_Indexed:
						{
							return FText::FromString("Asset Indexed, Meta Data not baked");
						}
					case EMetaDataBakingAssetStatus::MDBAS_Empty:
						{
							return FText::FromString("No Asset User Data");
						}
					default: break;
						}
                
                return FText::FromString("");
                    
            })
        ]
    ]
    // Cook Button column (Fixed width for alignment)
    + SHorizontalBox::Slot()
    .AutoWidth()
    .VAlign(VAlign_Center)
    [
        SNew(SButton)
        .ButtonStyle(FAppStyle::Get(), "SimpleButton")
        .ContentPadding(0)
        .ToolTipText(FText::FromString("Extracts the Meta Data into the Meta Data Storage Providers"))
        .IsEnabled_Lambda([this, ElementHandle]() -> bool
        {
        
            FString AssetPathString;
            FSoftObjectPath SoftAssetPath;
            if (ElementHandle->GetValueAsFormattedString(AssetPathString) == FPropertyAccess::Success)
            {
                SoftAssetPath = FSoftObjectPath(AssetPathString);
            }
            
            // Get the current status of the asset
            const UMetaDataIndexerSubsystem* Subsystem = GEditor->GetEditorSubsystem<UMetaDataIndexerSubsystem>();
            if (!Subsystem) return false;

            // Disable if the asset is Empty
            return Subsystem->GetSoftAssetStatus(SoftAssetPath) > EMetaDataBakingAssetStatus::MDBAS_Empty;
        })
        .OnClicked_Lambda([MetaDataCoreEditorSubsystem, MetaDataIndexerSubsystem, ElementHandle]() -> FReply
        {
            FString AssetPathString;
            FSoftObjectPath SoftAssetPath;
            if (ElementHandle->GetValueAsFormattedString(AssetPathString) == FPropertyAccess::Success)
            {
                SoftAssetPath = FSoftObjectPath(AssetPathString);
            }
            
            MetaDataCoreEditorSubsystem->RequestAssetBake(
                MetaDataIndexerSubsystem->GetBakingSettingForAsset(SoftAssetPath).LoadSynchronous(),
                SoftAssetPath);
            
            return FReply::Handled();
        })
        [
            SNew(SBox)
            .WidthOverride(20.0f)
            .HeightOverride(20.0f)
            [
                SNew(SImage)
                .Image(FAppStyle::GetBrush("MainFrame.CookContent"))
            ]
        ]
    ]
];        
    }


}