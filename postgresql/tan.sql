CREATE TABLE user_info(
       id bigserial PRIMARY KEY NOT NULL,
       name           TEXT    NOT NULL,
       type          varchar(6) NOT NULL,
       create_time  date
);


CREATE TABLE user_attention(
       id bigserial PRIMARY KEY NOT NULL,
       userid        bigint   NOT NULL,
       attention_userid  bigint NOT NULL,
       state         char(1)   NOT NULL,
       create_time   date,
       update_time   date
);


