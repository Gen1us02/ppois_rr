#pragma once

#include <sc-memory/sc_agent.hpp>
#include <map>

// Класс агента, реализуещего создание графа
class ScAgentCreateGraph : public ScActionInitiatedAgent
{
public:
  // Конструктор класса агента
  ScAgentCreateGraph();
  // Метод получения действия
  ScAddr GetActionClass() const override;
  // Основной метод, описывающий основную логику агента. В нем происходит создание графа
  ScResult DoProgram(ScActionInitiatedEvent const & event, ScAction & action) override;
  // Метод, которой проверяет есть ли узел в sc-памяти.
  // Если есть он присваивает pageNode адрес этого узла, иначе создает его
  void GetOrCreatePage(ScAddr & pageNode, std::string const & pageName);
  // Создает основной идентификатор для узла
  void CreateMainIdntf(ScAddr const & node, std::string const & idtf);

private:
  int next_page{0};
  std::map<std::string, std::string> pagesIdentificators_;
};