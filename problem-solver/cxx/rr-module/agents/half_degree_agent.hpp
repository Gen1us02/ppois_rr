#pragma once

#include <sc-memory/sc_agent.hpp>
#include <map>
#include <algorithm>

using SuccessfulyGraphCreationEvent = ScEventAfterGenerateOutgoingArc<ScType::ConstPermPosArc>;

// Класс агента, который находит тупиковые страницы, страницы источники, популярные страницы
class ScHalfDegreeAgent : public ScAgent<SuccessfulyGraphCreationEvent>
{
public:
  // Конструктор агента
  ScHalfDegreeAgent();
  // Метод для получения действия
  ScAddr GetActionClass() const override;
  // Метод, возвращающий элемент, появление которого инициирует действие
  ScAddr GetEventSubscriptionElement() const override;
  // Метод, возвращающий шаблон, по которому инициируется действие
  ScTemplate GetInitiationConditionTemplate(SuccessfulyGraphCreationEvent const & event) const override;
  // Основной метод работы агента, который находит тупиковые страницы, страницы источники, популярные страницы
  ScResult DoProgram(SuccessfulyGraphCreationEvent const & event, ScAction & action) override;
  // Метод, проверяющий наличие ролевого отношения между структурой сайта и страницей
  ScAddr GetStructConnector(ScAddr const & page, ScStructure & graphStruct);
  // Метод получения вектора всех страниц
  void GetPagesVector();
  // Метод для создания ссылок для хранения числа полустепеней входа/выхода
  void CreateDegreeLinks(ScAddr const & page, int number, std::string const & type);
  // Метод создания ролевых отношений между тупиковыми страницами и структурой сайта
  void CreateDeadEndRelations(ScAddr const & page, ScStructure & graphStruct);
  // Метод создания ролевых отношений между популярными страницами(страницами источниками) и структурой сайта
  void CreateSourceAndPopularRelations(int const & popularMax, int const & sourceMax, ScStructure & graphStruct);

private:
  ScAddrVector pages_;
  std::map<int, ScAddrUnorderedSet> popularPages_;
  std::map<int, ScAddrUnorderedSet> sourcePages_;
};