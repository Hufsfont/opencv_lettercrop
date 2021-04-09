# opencv_lettercrop
opencv_lettercrop 최종본(정리)
## 1. 프로젝트 소개
### 프로젝트 주제
OpenCV를 이용하여 필기체 이미지에서 한글 문자영역을 인식하여 지정하고 추출하는 기법
### 프로젝트 목적
규격문자 이외에 필기체같은 비규격 문자에서 문자 인식의 정확성을 상승
### 제한 사항
- 원고지 모양 이미지를 사용하여 글자 검출률을 높였으나 상용화하기 어렵다.
- 이미지 해상도에 따라 오류가 많다.
## 2. 시스템 구성
### 개발환경
|구성|환경|
|:---:|:---:|
|Version|Opencv-4.40|
|Build 환경|Visual studio 2019| 
|Language|C language| 
|운영체제|Window10(x64)| 
### 프로그램 순서도
<img src="https://user-images.githubusercontent.com/64073715/114119745-3a3fd180-9926-11eb-93ef-afeded9e24d0.png" width="150" height="500">

## 3. 결과 데모
<img src="https://user-images.githubusercontent.com/64073715/114120096-d2d65180-9926-11eb-8e3d-7fd8df5f00f7.png" width="500" height="200">

보이는 초록색 선을 따라 이미지가 crop되어 num[0~9].jpg로 저장된다.

