#pragma once
#include "stdafx.h"
#include "inRange.h"
array OldYaeAlgorithm(array subtitleFrame) {
	//ขั้นที่ 1: threshold สีดำ
	array blackMask = inRange(subtitleFrame, 0, 0, 0, 20, 20, 20);
	//ขั้น 2: สลับสี
	array invertedMask = !blackMask;

	//ขั้น 3: ถมดำ
	array cavityMask = regions(invertedMask.as(b8), AF_CONNECTIVITY_8);
	array cavityMaskBorder = join(0, flat(cavityMask.row(0)), cavityMask.col(0), cavityMask.col(end), flat(cavityMask.row(end)));
	cavityMaskBorder = setUnique(cavityMaskBorder).as(s32);
	int i, borderRegionCount = cavityMaskBorder.dims(0);
	int* borderRegions = cavityMaskBorder.host<int>();
	for (i = 0; i < borderRegionCount; i++) {
		cavityMask -= (cavityMask == borderRegions[i]) * borderRegions[i];
	}
	//ขั้นที่ 4: หาพื้นที่ใหญ่เกินไปในภาพ
	array largeObjectImage = erode(cavityMask, constant(1, 9, 9));
	//ขั้นที่ 5: ถมวัตถุที่ใหญ่เกินไป
	array largeObjectSet = setUnique(flat(largeObjectImage)).as(s32);
	int largeObjectCount = largeObjectSet.dims(0);
	int* largeObjectIds = largeObjectSet.host<int>();
	for (i = 0; i < largeObjectCount; i++) {
		cavityMask -= (cavityMask == largeObjectIds[i]) *largeObjectIds[i];
	}
	//ขั้นที่ 6: ลบพื้นที่เล็กเกินไปออกจากภาพ
	array smallMask = constant(1, 3, 3);
	array denoisedImage = dilate(erode(cavityMask, smallMask), smallMask);
	//ขั้น 7: นำพื้นที่ขนาดเล็กออกจาก cavityMask ด้วย connected component labeling เพื่อไม่ให้เสียหายมาก
	array denoisedObjectSet = setUnique(flat(denoisedImage)).as(s32);
	array resultMask = constant(0, cavityMask.dims(0), cavityMask.dims(1));
	int denoisedObjectCount= denoisedObjectSet.dims(0);
	int* denoiseObjectIds = denoisedObjectSet.host<int>();
	for (i = 0; i < denoisedObjectCount; i++) {
		resultMask += (cavityMask == denoiseObjectIds[i]) * denoiseObjectIds[i];
	}
	return resultMask > 0;
}