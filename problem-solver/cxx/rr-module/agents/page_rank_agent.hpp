#pragma once

#include <sc-memory/sc_agent.hpp>
#include <map>
#include <algorithm>
#include <string>

using SuccessfulyGraphCreationEvent = ScEventAfterGenerateOutgoingArc<ScType::ConstPermPosArc>;

// Класс агента, которой вычисляет PageRank для всех страниц, ранжирует их по важности
class ScPageRankAgent : public ScAgent<SuccessfulyGraphCreationEvent>
{
public:
  // Конструктор агента
  ScPageRankAgent();
  // Метод, который получает класс действия
  ScAddr GetActionClass() const override;
  // Метод, возвращающий элемент, появление которого инициирует действие
  ScAddr GetEventSubscriptionElement() const override;
  // Метод, возвращающий шаблон, по которому инициируется действие
  ScTemplate GetInitiationConditionTemplate(SuccessfulyGraphCreationEvent const & event) const override;
  // Основной метод работы агента, который вычисляет PageRank и ранжирует страницы по важности
  ScResult DoProgram(SuccessfulyGraphCreationEvent const & event, ScAction & action) override;
  // Метод получения вектора всех страниц
  void GetPagesVector();
  // Метод вычисления PageRank
  void CalculatePageRank();
  // Метод ранжирования страниц по PageRank
  void GeneratePageRankLinks(int place, double pageRank, ScAddr const & page);

private:
  ScAddrVector pages_;
  std::map<double, std::string> pageRanks_;
};