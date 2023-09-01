%option noyywrap
%{    
        #pragma once
        #include<mutex>
        #include<vector>
        #include<stdio.h>
        using sql = parser::sql;
        std::vector<sql>* __plex;
%}
%%
SELECT  __plex->push_back(sql::SELECT);
WHERE   __plex->push_back(sql::WHERE);
FROM    __plex->push_back(sql::FROM);
AND     __plex->push_back(sql::AND);
OR      __plex->push_back(sql::OR);
CREATE  __plex->push_back(sql::CREATE);
DROP    __plex->push_back(sql::DROP);
TABLE   __plex->push_back(sql::TABLE);
%%
void __run_lexer(FILE* fp)
{
    yyin = fp;
    yylex();
}
