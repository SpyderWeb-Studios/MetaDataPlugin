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
    void OnToggleChanged(ECheckBoxState CheckBoxState);
    ECheckBoxState IsButtonChecked() const;
    void OnButtonToggleChanged(ECheckBoxState CheckBoxState);
	virtual void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override;
    bool bToggleState;
};

inline TSharedRef<IDetailCustomization> FMetaDataSettingsCustomisation::MakeInstance()
{
	return MakeShared<FMetaDataSettingsCustomisation>();
}

inline ECheckBoxState FMetaDataSettingsCustomisation::IsButtonChecked() const
{
    return bToggleState ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;
}

inline void FMetaDataSettingsCustomisation::OnButtonToggleChanged(ECheckBoxState NewState)
{
    bToggleState = (NewState == ECheckBoxState::Checked);
    
    // Your toggle logic goes here
}

inline void FMetaDataSettingsCustomisation::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
    TArray<TWeakObjectPtr<UObject>> Objects;
    DetailBuilder.GetObjectsBeingCustomized(Objects);

    TSoftObjectPtr<UMetaDataBakingSettingsDataAsset> SoftDataAssetPath;
    
    if (Objects.Num() > 0 && Objects[0].IsValid())
    {
        // Cast it to your specific class
        TWeakObjectPtr<UObject> ActiveDataAsset = Objects[0];

        if(ActiveDataAsset.IsStale())
        {
            return;
        }

        SoftDataAssetPath = Cast<UMetaDataBakingSettingsDataAsset>(ActiveDataAsset.Get());
    }


    UMetaDataIndexerSubsystem* MetaDataIndexerSubsystem = GEditor->GetEditorSubsystem<UMetaDataIndexerSubsystem>();
    UMetaDataEditorSubsystem* MetaDataCoreEditorSubsystem = GEditor->GetEditorSubsystem<UMetaDataEditorSubsystem>();
    check(MetaDataIndexerSubsystem);
    
    
    IDetailCategoryBuilder& BakingCategory = DetailBuilder.EditCategory("Baking");

    BakingCategory.AddCustomRow(FText::FromString("Actions"))
    .WholeRowContent()
    [
        SNew(SBorder)
        .BorderImage(FAppStyle::GetBrush("ToolPanel.GroupBorder"))
        .Padding(4.0f)
        [
            SNew(SHorizontalBox)
            + SHorizontalBox::Slot()
            .AutoWidth()
            .Padding(0, 0, 8, 0)
            [
                SNew(SButton)
                .ButtonStyle(FAppStyle::Get(), "PrimaryButton") // Gives it the blue 'Action' look
                .Text(FText::FromString("Refresh Cache"))
                .ToolTipText(FText::FromString("Scan folders for new assets"))
                .OnClicked_Lambda([MetaDataIndexerSubsystem, SoftDataAssetPath]() -> FReply
                {
                    MetaDataIndexerSubsystem->RefreshDataAssetCache(SoftDataAssetPath.LoadSynchronous());
                    return FReply::Handled();
                })
            ]
            + SHorizontalBox::Slot()
            .AutoWidth()
            [
                SNew(SButton)
                .ButtonStyle(FAppStyle::Get(), "PrimaryButton") // Gives it the blue 'Action' look
                .Text(FText::FromString("Bake All"))
                .ToolTipText(FText::FromString("Run the baking process"))
                .OnClicked_Lambda([MetaDataCoreEditorSubsystem, SoftDataAssetPath]() -> FReply
                {
                    MetaDataCoreEditorSubsystem->RequestBakerySettingBake(SoftDataAssetPath.LoadSynchronous());
                    return FReply::Handled();
                })
            ]
            
        ]
    ];
    TSharedPtr<IPropertyHandle> ExternalBakingSettingProperty = DetailBuilder.GetProperty("ExternalBakingSetting");
    BakingCategory.AddProperty(ExternalBakingSettingProperty);
    
    
    DetailBuilder.EditCategory("Routing");
    DetailBuilder.EditCategory("Naming");
    
	TSharedPtr<IPropertyHandle> MapProperty = DetailBuilder.GetProperty("CachedAssets");
    IDetailCategoryBuilder& Category = DetailBuilder.EditCategory("Cache");

    // Hide the default map view
    Category.AddProperty(MapProperty).Visibility(EVisibility::Collapsed);
  
    // Create a custom UI for the map items
    TSharedPtr<IPropertyHandleSet> MapHandle = MapProperty->AsSet();
    uint32 NumChildren = 0;
    MapProperty->GetNumChildren(NumChildren);
    
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
