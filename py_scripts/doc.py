import re

def parse_multiline_string(input_str):
    patterns = re.findall(r'lua_pushtablestringstring\(L, "(.*?)", (.*?);', input_str, re.DOTALL)

    output_lines = []
    for name, value in patterns:
        output_lines.append(f'  - name: {name}\n'
                           # f'    type: {type(value).__name__}\n'
                           f'    type: string\n'
                           f'    desc: Predefined event\n')

    return '\n'.join(output_lines)

# Example usage
multiline_input = """
    lua_pushtablestringstring(L, "EVENT_ADIMPRESSION", kEventAdImpression);
    lua_pushtablestringstring(L, "EVENT_ADDPAYMENTINFO", kEventAddPaymentInfo);
    lua_pushtablestringstring(L, "EVENT_ADDSHIPPINGINFO", kEventAddShippingInfo);
    lua_pushtablestringstring(L, "EVENT_ADDTOCART", kEventAddToCart);
    lua_pushtablestringstring(L, "EVENT_ADDTOWISHLIST", kEventAddToWishlist);
    lua_pushtablestringstring(L, "EVENT_APPOPEN", kEventAppOpen);
    lua_pushtablestringstring(L, "EVENT_BEGINCHECKOUT", kEventBeginCheckout);
    lua_pushtablestringstring(L, "EVENT_CAMPAIGNDETAILS", kEventCampaignDetails);
    lua_pushtablestringstring(L, "EVENT_EARNVIRTUALCURRENCY", kEventEarnVirtualCurrency);
    lua_pushtablestringstring(L, "EVENT_GENERATELEAD", kEventGenerateLead);
    lua_pushtablestringstring(L, "EVENT_JOINGROUP", kEventJoinGroup);
    lua_pushtablestringstring(L, "EVENT_LEVELEND", kEventLevelEnd);
    lua_pushtablestringstring(L, "EVENT_LEVELSTART", kEventLevelStart);
    lua_pushtablestringstring(L, "EVENT_LEVELUP", kEventLevelUp);
    lua_pushtablestringstring(L, "EVENT_LOGIN", kEventLogin);
    lua_pushtablestringstring(L, "EVENT_POSTSCORE", kEventPostScore);
    lua_pushtablestringstring(L, "EVENT_PURCHASE", kEventPurchase);
    lua_pushtablestringstring(L, "EVENT_REFUND", kEventRefund);
    lua_pushtablestringstring(L, "EVENT_REMOVEFROMCART", kEventRemoveFromCart);
    lua_pushtablestringstring(L, "EVENT_SCREENVIEW", kEventScreenView);
    lua_pushtablestringstring(L, "EVENT_SEARCH", kEventSearch);
    lua_pushtablestringstring(L, "EVENT_SELECTCONTENT", kEventSelectContent);
    lua_pushtablestringstring(L, "EVENT_SELECTITEM", kEventSelectItem);
    lua_pushtablestringstring(L, "EVENT_SELECTPROMOTION", kEventSelectPromotion);
    lua_pushtablestringstring(L, "EVENT_SHARE", kEventShare);
    lua_pushtablestringstring(L, "EVENT_SIGNUP", kEventSignUp);
    lua_pushtablestringstring(L, "EVENT_SPENDVIRTUALCURRENCY", kEventSpendVirtualCurrency);
    lua_pushtablestringstring(L, "EVENT_TUTORIALBEGIN", kEventTutorialBegin);
    lua_pushtablestringstring(L, "EVENT_TUTORIALCOMPLETE", kEventTutorialComplete);
    lua_pushtablestringstring(L, "EVENT_UNLOCKACHIEVEMENT", kEventUnlockAchievement);
    lua_pushtablestringstring(L, "EVENT_VIEWCART", kEventViewCart);
    lua_pushtablestringstring(L, "EVENT_VIEWITEM", kEventViewItem);
    lua_pushtablestringstring(L, "EVENT_VIEWITEMLIST", kEventViewItemList);
    lua_pushtablestringstring(L, "EVENT_VIEWPROMOTION", kEventViewPromotion);
    lua_pushtablestringstring(L, "EVENT_VIEWSEARCHRESULTS", kEventViewSearchResults);
    lua_pushtablestringstring(L, "PARAM_ADFORMAT", kParameterAdFormat);
    lua_pushtablestringstring(L, "PARAM_ADNETWORKCLICKID", kParameterAdNetworkClickID);
    lua_pushtablestringstring(L, "PARAM_ADPLATFORM", kParameterAdPlatform);
    lua_pushtablestringstring(L, "PARAM_ADSOURCE", kParameterAdSource);
    lua_pushtablestringstring(L, "PARAM_ADUNITNAME", kParameterAdUnitName);
    lua_pushtablestringstring(L, "PARAM_AFFILIATION", kParameterAffiliation);
    lua_pushtablestringstring(L, "PARAM_CP1", kParameterCP1);
    lua_pushtablestringstring(L, "PARAM_CAMPAIGN", kParameterCampaign);
    lua_pushtablestringstring(L, "PARAM_CAMPAIGNID", kParameterCampaignID);
    lua_pushtablestringstring(L, "PARAM_CHARACTER", kParameterCharacter);
    lua_pushtablestringstring(L, "PARAM_CONTENT", kParameterContent);
    lua_pushtablestringstring(L, "PARAM_CONTENTTYPE", kParameterContentType);
    lua_pushtablestringstring(L, "PARAM_COUPON", kParameterCoupon);
    lua_pushtablestringstring(L, "PARAM_CREATIVEFORMAT", kParameterCreativeFormat);
    lua_pushtablestringstring(L, "PARAM_CREATIVENAME", kParameterCreativeName);
    lua_pushtablestringstring(L, "PARAM_CREATIVESLOT", kParameterCreativeSlot);
    lua_pushtablestringstring(L, "PARAM_CURRENCY", kParameterCurrency);
    lua_pushtablestringstring(L, "PARAM_DESTINATION", kParameterDestination);
    lua_pushtablestringstring(L, "PARAM_DISCOUNT", kParameterDiscount);
    lua_pushtablestringstring(L, "PARAM_ENDDATE", kParameterEndDate);
    lua_pushtablestringstring(L, "PARAM_EXTENDSESSION", kParameterExtendSession);
    lua_pushtablestringstring(L, "PARAM_FLIGHTNUMBER", kParameterFlightNumber);
    lua_pushtablestringstring(L, "PARAM_GROUPID", kParameterGroupID);
    lua_pushtablestringstring(L, "PARAM_INDEX", kParameterIndex);
    lua_pushtablestringstring(L, "PARAM_ITEMBRAND", kParameterItemBrand);
    lua_pushtablestringstring(L, "PARAM_ITEMCATEGORY", kParameterItemCategory);
    lua_pushtablestringstring(L, "PARAM_ITEMCATEGORY2", kParameterItemCategory2);
    lua_pushtablestringstring(L, "PARAM_ITEMCATEGORY3", kParameterItemCategory3);
    lua_pushtablestringstring(L, "PARAM_ITEMCATEGORY4", kParameterItemCategory4);
    lua_pushtablestringstring(L, "PARAM_ITEMCATEGORY5", kParameterItemCategory5);
    lua_pushtablestringstring(L, "PARAM_ITEMID", kParameterItemID);
    lua_pushtablestringstring(L, "PARAM_ITEMLISTID", kParameterItemListID);
    lua_pushtablestringstring(L, "PARAM_ITEMLISTNAME", kParameterItemListName);
    lua_pushtablestringstring(L, "PARAM_ITEMNAME", kParameterItemName);
    lua_pushtablestringstring(L, "PARAM_ITEMVARIANT", kParameterItemVariant);
    lua_pushtablestringstring(L, "PARAM_ITEMS", kParameterItems);
    lua_pushtablestringstring(L, "PARAM_LEVEL", kParameterLevel);
    lua_pushtablestringstring(L, "PARAM_LEVELNAME", kParameterLevelName);
    lua_pushtablestringstring(L, "PARAM_LOCATION", kParameterLocation);
    lua_pushtablestringstring(L, "PARAM_LOCATIONID", kParameterLocationID);
    lua_pushtablestringstring(L, "PARAM_MARKETINGTACTIC", kParameterMarketingTactic);
    lua_pushtablestringstring(L, "PARAM_MEDIUM", kParameterMedium);
    lua_pushtablestringstring(L, "PARAM_METHOD", kParameterMethod);
    lua_pushtablestringstring(L, "PARAM_NUMBEROFNIGHTS", kParameterNumberOfNights);
    lua_pushtablestringstring(L, "PARAM_NUMBEROFPASSENGERS", kParameterNumberOfPassengers);
    lua_pushtablestringstring(L, "PARAM_NUMBEROFROOMS", kParameterNumberOfRooms);
    lua_pushtablestringstring(L, "PARAM_ORIGIN", kParameterOrigin);
    lua_pushtablestringstring(L, "PARAM_PAYMENTTYPE", kParameterPaymentType);
    lua_pushtablestringstring(L, "PARAM_PRICE", kParameterPrice);
    lua_pushtablestringstring(L, "PARAM_PROMOTIONID", kParameterPromotionID);
    lua_pushtablestringstring(L, "PARAM_PROMOTIONNAME", kParameterPromotionName);
    lua_pushtablestringstring(L, "PARAM_QUANTITY", kParameterQuantity);
    lua_pushtablestringstring(L, "PARAM_SCORE", kParameterScore);
    lua_pushtablestringstring(L, "PARAM_SCREENCLASS", kParameterScreenClass);
    lua_pushtablestringstring(L, "PARAM_SCREENNAME", kParameterScreenName);
    lua_pushtablestringstring(L, "PARAM_SEARCHTERM", kParameterSearchTerm);
    lua_pushtablestringstring(L, "PARAM_SHIPPING", kParameterShipping);
    lua_pushtablestringstring(L, "PARAM_SHIPPINGTIER", kParameterShippingTier);
    lua_pushtablestringstring(L, "PARAM_SOURCE", kParameterSource);
    lua_pushtablestringstring(L, "PARAM_SOURCEPLATFORM", kParameterSourcePlatform);
    lua_pushtablestringstring(L, "PARAM_STARTDATE", kParameterStartDate);
    lua_pushtablestringstring(L, "PARAM_SUCCESS", kParameterSuccess);
    lua_pushtablestringstring(L, "PARAM_TAX", kParameterTax);
    lua_pushtablestringstring(L, "PARAM_TERM", kParameterTerm);
    lua_pushtablestringstring(L, "PARAM_TRANSACTIONID", kParameterTransactionID);
    lua_pushtablestringstring(L, "PARAM_TRAVELCLASS", kParameterTravelClass);
    lua_pushtablestringstring(L, "PARAM_VALUE", kParameterValue);
    lua_pushtablestringstring(L, "PARAM_VIRTUALCURRENCYNAME", kParameterVirtualCurrencyName);
    lua_pushtablestringstring(L, "PROP_ALLOWADPERSONALIZATIONSIGNALS", kUserPropertyAllowAdPersonalizationSignals);
    lua_pushtablestringstring(L, "PROP_SIGNUPMETHOD", kUserPropertySignUpMethod);
"""

output = parse_multiline_string(multiline_input)
print(output)