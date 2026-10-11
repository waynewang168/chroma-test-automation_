# ATE Test Automation & Vector Engine (AI-Assisted)

[![Language: C](https://img.shields.io/badge/Language-C-blue.svg)](https://en.wikipedia.org/wiki/C_(programming_language))
[![Workflow: Git Branching](https://img.shields.io/badge/Workflow-Feature%20Branch-orange.svg)]()
[![AI-Assisted: Claude](https://img.shields.io/badge/AI--Assisted-Claude-purple.svg)]()

## 專案簡介 (Overview)
本專案模擬半導體自動化測試機台（ATE）之晶片測試向量（Test Vectors）比對引擎與供電防呆機制。結合狀態機控制邏輯與現代化 AI 協作開發工作流，旨在展示軟硬體整合測試、邊界異常隔離以及規範化版本控制實踐。

---

## 核心架構與功能 (Key Features)

### 1. 測試向量比對模型 (Test Vector Verification)
* 建立 8-Pin 週期（Cycle-by-Cycle）訊號比對結構體，模擬機台對晶片輸入激勵向量（Input Pattern）並比對預期響應（Expected Pattern）。
* 具備時序週期追蹤與 Pass/Fail 判定邏輯。

### 2. 邊界欠壓鎖定保護 (UVLO Protection Logic)
* 針對電源瞬態異常實作硬體防呆模擬：即時監控供電電壓（Supply Voltage）。
* 當輸入電壓低於安全閾值（3.0V）時觸發 UVLO 欠壓保護，立即阻斷測試並回傳錯誤代碼，防止機台誤殺或待測晶片損壞。

### 3. 現代化 AI 協作開發 (AI-Assisted Development Workflow)
* **Prompt-Driven Architecture**：運用 Claude 進行提示詞驅動開發，快速搭建邊界測試案例（Boundary Test Cases）與極端條件模擬。
* **Feature Branch Workflow**：嚴格遵循 Git 分支規範，透過獨立功能分支（`feature/uvlo-protection`）完成邏輯驗證後併入主線。

---

## 測試案例與輸出 (Execution Results)

```text
Executing Test Verification...
[PASS] Cycle 1: In=10100101, Exp=01011010 (Voltage: 3.30V)
[FAIL] Cycle 2: Voltage Sag (2.60V < 3.0V) -> UVLO Triggered! Test Aborted.
