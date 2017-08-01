--#ENDPOINT POST /device/setValues
response.message = {}

for k, v in pairs(request.body) do
  response.message[k] = Lampy.setIdentityState({
    identity = "90:A2:DA:F3:1E:1E",
    print(k),
    print(v),
    [k] = tonumber(v)
  })
end

return response.message
