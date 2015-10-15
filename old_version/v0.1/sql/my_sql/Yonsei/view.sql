/*
==============================================================================
	TOTAL
==============================================================================
*/
/* ---------------------------------------------------------------------
	ALUMNI
--------------------------------------------------------------------- */
SELECT  
	A.name, A.mobile, A.mail, A.note, A.position, A.major, D.summaryName
FROM Yonsei.alumni A, Dart.corporation D
WHERE A.corporateKey = D.corporateKey
ORDER BY A.major, A.name;

/* ---------------------------------------------------------------------
	CORPORATION
--------------------------------------------------------------------- */
SELECT 
	D.summaryName, 
	C.president, C.category, C.mainProduct, C.size, C.employed, 
	C.establishedYear, C.settledYear,
	C.asset, C.sales,
	C.address, C.tel, C.fax
FROM Yonsei.corporation C, Dart.corporation D
WHERE C.corporateKey AND C.corporateKey = D.corporateKey
ORDER BY C.sales DESC, C.asset DESC;

/* ---------------------------------------------------------------------
	ALUMNI + CORPORATION
--------------------------------------------------------------------- */
SELECT  
	A.name, A.mobile, A.mail, A.note, A.position, A.major, D.summaryName,
	C.president, C.category, C.mainProduct, C.size, C.employed, 
	C.establishedYear, C.settledYear,
	C.asset, C.sales,
	C.address, C.tel, C.fax
FROM Yonsei.alumni A, Yonsei.corporation C, Dart.corporation D
WHERE A.corporateKey = C.corporateKey AND C.corporateKey = D.corporateKey
ORDER BY C.sales DESC, C.asset DESC, A.major, A.name;

/*
==============================================================================
	LISTED CORPORATION
==============================================================================
*/
CREATE VIEW Yonsei.v_listedCorporation
AS
SELECT 
#ȸ�� ����
	C.summaryName corporateName, C.code, C.market, C.address, C.homepage, C.tel, C.fax, C.establishedDate, C.settlingMonth,

#�繫����
	#�ð��Ѿ�, �����
		R.marketCap, S.sales,
	#���ͼ�
		R.operatingProfitRatio, R.netProfitRatio, R.ROE, R.ROA, R.ROIC,
	#���强
		R.saleGrowth, R.operatingProfitGrowth, R.netProfitGrowth, 
		totalAssetGrowth, currentAssetGrowth, tangibleAssetGrowth, capitalGrowth,
	#������
		R.debtRatio, R.currentDebtRatio, R.illiquidDebtRatio, R.netDebtRatio, 
		R.currentRatio, R.quickRatio, R.interestCoverageRatio, R.financialCostRatio, R.reverseRatio, 
	#Ȱ����
		R.totalAssetTurnoverRatio, R.receivableTurnoverRatio, R.inventoryTurnoverRatio, R.payableTurnoverRatio,
	#��ġ��ǥ
		R.EPS, R.BPS, R.CFPS, R.SPS, R.PER, R.PBR, R.PCR, R.PSR,
		R.EV_EBITDA, R.modifiedDPS, R.commonStockDividentRatio, R.payOutRatio,
		R.dateYear, R.dateQuarter,

	S.standard

FROM Yonsei.corporation L, Dart.corporation C, Report.sales S, Company.mv_retrieve R
WHERE C.market IN ('�������ǽ���', '�ڽ��ڽ���') AND
	L.corporateKey = C.corporateKey AND
	C.code = S.code AND
	S.code = R.code AND S.standard = R.standard AND S.date = R.dateYear AND
		S.period = 4;